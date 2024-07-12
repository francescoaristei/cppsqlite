# include "Page.h"
# include "Table.h"
# include "Row.h"
# include "AbstractDataType.h"
# include "Exceptions.h"
# include "utils.h"
# include <iostream>
# include <vector>
# include <map>


Table::Table (map<string, AbstractDataType*> datatypes, vector<string> attrNames, string name) {
    this -> datatypes = datatypes;
    this -> attrNames = attrNames;
    uint32_t rowSize = 0;
    for (map<string, AbstractDataType*>::iterator itr = this -> datatypes.begin(); itr != this -> datatypes.end(); ++itr) {
        rowSize += itr->second->getSize();
    }
    /* initialize the pager for the table */
    this -> pager = new Pager("cpp/src/database/" + name + ".txt", rowSize, datatypes);

    /* will be changed when loading the data saved permanently to disk with the number of rows of the stored table */
    this -> numRows = this -> pager -> getNumRows();
}

Table::~Table () {
    for (map<string, AbstractDataType*>::iterator itr = this -> datatypes.begin(); itr != this -> datatypes.end(); ++itr) {
        delete itr->second;
    }
}


Pager* Table::getPager () {
    return this -> pager;
}

vector<Page*> Table::getPages () {
    return this -> pager -> getPages();
}

/* temporary */
uint32_t Table::getNumRows () {
    return this -> numRows;
}

/* temporary */
void Table::setNumRows () {
    this -> numRows += 1;
}

map<string, AbstractDataType*> Table::getDatatypes () {
    return this -> datatypes;
}

void Table::insertRow (Row *row) {
    this -> pager -> insertRow(row, this -> numRows);
    this -> numRows = this -> pager -> getNumRows();
}


vector<Row*> Table::getRows () {
    return this -> pager -> getRows(this -> attrNames, this -> numRows);
}


