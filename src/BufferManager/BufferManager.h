//
// Created by Haotian on 17/6/20.
//

#ifndef MINISQL_BUFFERMANAGER_H
#define MINISQL_BUFFERMANAGER_H

#include <string>
using namespace std;

class BufferManager {
public:
    BufferManager() = default;
    ~BufferManager() = default;
    void removeFileNode(string filename);
    void removeFile(string filename);
    char *getFileBlock(string filename);
    void setDirty(const char *dest);
};

#endif //MINISQL_BUFFERMANAGER_H
