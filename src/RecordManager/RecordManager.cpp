//
// Created by Haotian on 17/5/25.
//

#include "RecordManager.h"
#include "../../include/DataStructure.h"

bool RecordManager::createTable(const string &table) {
    string tableFileStr = tableFile(table);
    bm->createFile(tableFileStr);
    return true;
}

bool RecordManager::dropTable(const string &table) {
    string tableFileStr = tableFile(table);
    bm->removeFile(tableFileStr);
    return true;
}

bool RecordManager::createIndex(const Table &table, const SqlValueType &index) {
    string indexFileStr = indexFile(table.Name, index.attrName);
    // Call BM to create index file
    bm->createFile(indexFileStr);
    // Call IM to create index tree
    im->create(indexFileStr, index);
    // Add initial values to index tree
    int blockID = 0;
    char *block = bm->getBlock(tableFile(table.Name), blockID);
    int length = table.recordLength + 1;
    int recordsPerBlock = BlockSize / length;
    int offset = 1;
    Tuple tup;
    Element attr;
    const char *dest;

    for (auto const &attrType : table.attrType) {
        if (attrType.attrName == index.attrName) {
            attr.type = attrType;
            break;
        }
        offset += attrType.getSize();
    }

    while (block) {
        for (int i = 0; i < recordsPerBlock; i++) {
            if (block[i * length] != Used) { continue; }
            dest = block + i * length + offset;
            switch (attr.M()) {
                case MINISQL_TYPE_INT:
                    memcpy(&attr.i, dest, attr.type.getSize());
                    break;
                case MINISQL_TYPE_FLOAT:
                    memcpy(&attr.r, dest, attr.type.getSize());
                    break;
                case MINISQL_TYPE_CHAR:
                    memcpy(const_cast<char *>(attr.str.c_str()), dest, attr.type.getSize());
                    break;
                default:
                    cerr << "Undefined type in RM::createIndex." << endl;
            }
            im->insert(indexFileStr, attr, blockID * recordsPerBlock + i);
        }
        bm->setFree(tableFile(table.Name), blockID);
        blockID++;
        block = bm->getBlock(tableFile(table.Name), blockID);
    }
    return true;
}

bool RecordManager::dropIndex(const Table &table, const string &index) {
    string indexFileStr = indexFile(table.Name, index);
    bm->removeFile(indexFileStr);

    bool foundAttr = false;

    for (auto &attr : table.attrType) {
        if (attr.attrName == index) {
            foundAttr = true;
            im->drop(indexFileStr, attr);
            break;
        }
    }
    if (!foundAttr) {
        cerr << "Drop index on undefined attr!" << endl;
    }

    return true;
}

int RecordManager::insertRecord(const Table &table, const Tuple &record) {
    string tableName = tableFile(table.Name);
    int blockID = bm->getBlockTail(tableName);
    char *block;
    if (blockID < 0) {
        blockID = 0;
        block = bm->getBlock(tableName, blockID, true);
    } else {
        block = bm->getBlock(tableName, blockID);
    }
    int length = table.recordLength + 1;
    int recordsPerBlock = BlockSize / length;
    int offset = 1;
    string strFixed;
    int lengthOffset;

    bool validBlock = false;

    int recordOffset = 0;

    while (recordOffset < recordsPerBlock) {
        if (block[recordOffset * length] != UnUsed) {
            recordOffset++;
            continue;
        }
        validBlock = true;
        block += recordOffset * length;
        break;
    }

    if (!validBlock) {
        recordOffset = 0;
        bm->setFree(tableName, blockID);
        block = bm->getBlock(tableName, ++blockID, true); // get next block (should be empty) and get the first unit
    }

    for (auto attr = record.element.begin(); attr < record.element.end(); attr++) {
        switch (attr->type.M()) {
            case MINISQL_TYPE_CHAR:
                strFixed = attr->str;
                lengthOffset = attr->type.charSize - strFixed.length();
                if (lengthOffset > 0) {
                    strFixed.insert(strFixed.length(), lengthOffset, 0);
                }
                memcpy(block + offset, strFixed.c_str(), attr->type.charSize + 1);
                offset += attr->type.charSize + 1;
                break;
            case MINISQL_TYPE_INT:
                memcpy(block + offset, &attr->i, sizeof(int));
                offset += sizeof(int);
                break;
            case MINISQL_TYPE_FLOAT:
                memcpy(block + offset, &attr->r, sizeof(float));
                offset += sizeof(float);
                break;
            default:
                cerr << "Undefined type!!" << endl;
                break;
        }
    }
    block[0] = Used;
    bm->setDirty(tableName, blockID);
    bm->setFree(tableName, blockID);
    return blockID * recordsPerBlock + recordOffset;
}

int RecordManager::selectRecord(const Table &table, const vector<string> &attr, const vector<Cond> &cond) {
    int blockID = 0;
    char *block = bm->getBlock(tableFile(table.Name), blockID);
    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    Tuple tup;
    Row row;
    Result res;

    while (block) {
        for (int i = 0; i < blocks; i++) {
            if (block[i * length] != Used) { continue; }
            convertToTuple(block, i * length, table.attrType, tup);
            if (condsTest(cond, tup, table.attrNames)) {
                row = tup.fetchRow(table.attrNames, attr);
                res.row.push_back(row);
            }
        }
        bm->setFree(tableFile(table.Name), blockID);
        blockID++;
        block = bm->getBlock(tableFile(table.Name), blockID);
    }

    dumpResult(res);
    return res.row.size();
}

int RecordManager::selectRecord(const Table &table, const vector<string> &attr, const vector<Cond> &cond,
                                 const IndexHint &indexHint, bool printResult) {
    string tableFileName = tableFile(table.Name);
    string indexFileName = indexFile(table.Name, indexHint.attrName);
    int recordPos;
    if (indexHint.cond.cond == MINISQL_COND_LESS || indexHint.cond.cond == MINISQL_COND_LEQUAL) {
        recordPos = im->searchHead(indexFileName, indexHint.attrType);
    } else {
        recordPos = im->search(indexFileName, indexHint.cond.value);
    }

    int length = table.recordLength + 1;
    int recordsPerBlock = BlockSize / length;
    char *block;
    int blockID;
    Tuple tup;
    Row row;
    Result res;
    Element e;
    bool degrade = false;
    int threshold = table.recordCnt / recordsPerBlock / 3;
    int cnt = 0;

    while (recordPos != -1) {
        blockID = recordPos / recordsPerBlock;
        block = bm->getBlock(tableFileName, blockID) + recordPos % recordsPerBlock * length;
        convertToTuple(block, 0, table.attrType, tup);
        if (condsTest(cond, tup, table.attrNames)) {
            row = tup.fetchRow(table.attrNames, attr);
            res.row.push_back(row);
        } else {
            e = tup.fetchElement(table.attrNames, indexHint.attrName);
            if (indexHint.cond.cond == MINISQL_COND_MORE) {
                IndexHint tmp = indexHint;
                tmp.cond.cond = MINISQL_COND_GEQUAL;
                if (!tmp.cond.test(e)) {
                    bm->setFree(tableFileName, blockID);
                    break;
                }
            } else if (!indexHint.cond.test(e)) {
                bm->setFree(tableFileName, blockID);
                break;
            }
        }
        recordPos = im->searchNext(indexFileName, indexHint.attrType);
        cnt++;
//        if (cnt > threshold) {
//            degrade = true;
//            bm->setFree(tableFileName, blockID);
//            break;
//        }
        bm->setFree(tableFileName, blockID);
    }

    if (!degrade) {
        if (printResult) {
            dumpResult(res);
        }
        return cnt;
    } else {
        return selectRecord(table, attr, cond);
    }
}

bool RecordManager::deleteRecord(const Table &table, const vector<Cond> &cond) {
    int blockOffset = 0;
    char *block = bm->getBlock(tableFile(table.Name), blockOffset);
    int length = table.recordLength + 1;
    int blocks = BlockSize / length;
    Tuple tup;

    while (block) {
        for (int i = 0; i < blocks; i++) {
            if (block[i * length] != Used) { continue; }
            convertToTuple(block, i * length, table.attrType, tup);
            if (condsTest(cond, tup, table.attrNames)) {
                block[i * length] = UnUsed;
                for (auto &col: tup.element) {
                    for (auto &attr : table.index) {
                        if (col.type.attrName == attr.first) {
                            im->removeKey(indexFile(table.Name, attr.first), col);
                        }
                    }
                }
            }
        }
        bm->setDirty(tableFile(table.Name), blockOffset);
        bm->setFree(tableFile(table.Name), blockOffset);
        blockOffset++;
        block = bm->getBlock(tableFile(table.Name), blockOffset);
    }
}

void RecordManager::dumpResult(const Result &res) const {
    for (auto const &row : res.row) {
        cout << " | ";
        for (auto const &col : row.col) {
            cout << col << " | ";
        }
        cout << endl;
    }
}

bool RecordManager::condsTest(const std::vector<Cond> &conds, const Tuple &tup, const std::vector<std::string> &attr) {
    int condPos;
    for (Cond cond : conds) {
        condPos = -1;
        for (int i = 0; i < attr.size(); i++) {
            if (attr[i] == cond.attr) {
                condPos = i;
                break;
            }
        }
        if (condPos == -1) {
            std::cerr << "Attr not found in cond test!" << std::endl;
        }
        if (!cond.test(tup.element[condPos])) {
            return false;
        }
    }
    return true;
}

void RecordManager::convertToTuple(const char *blockBuffer, int offset, const std::vector<SqlValueType> &attrType, Tuple &tup) {
    const char *block = blockBuffer + offset + 1; // 1 for meta bit
    Element e;
    tup.element.clear();
    for (int i = 0; i < attrType.size(); i++) {
        e.reset();
        e.type = attrType[i];
        switch (attrType[i].M()) {
            case MINISQL_TYPE_INT:
                memcpy(&e.i, block, sizeof(int));
                block += sizeof(int);
                break;
            case MINISQL_TYPE_FLOAT:
                memcpy(&e.r, block, sizeof(float));
                block += sizeof(float);
                break;
            case MINISQL_TYPE_CHAR:
                e.str = block;
                block += attrType[i].charSize + 1;
                break;
        }
        tup.element.push_back(e);
    }
}
