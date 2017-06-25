# Catalog Text File format convention

## meta file:
* each line for a table
* only a table name in each line

## Catalog file:
### for each attribute
* attribute name
* type: int char float
* size: 0 for int and float, 1~255 for char
* is primary key: 0 or 1
* is unique: 0 or 1
* is index: 0 or 1
* index name: a string, anything for no index, index name for index.
