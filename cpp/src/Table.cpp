
# include "Page.h"
# include "Table.h"
# include "Row.h"
# include "AbstractDataType.h"
# include "Exceptions.h"
# include "utils.h"
# include <iostream>
# include <vector>
# include <map>


Table::Table (map<string, AbstractDataType*> datatypes, vector<string> attrNames) : pages(TABLE_MAX_PAGE) {

    this -> datatypes = datatypes;
    this -> attrNames = attrNames;

    this -> rowSize = 0;
    for (map<string, AbstractDataType*>::iterator itr = this -> datatypes.begin(); itr != this -> datatypes.end(); ++itr) {
        rowSize += itr->second->getSize();
    }
    this -> rowsPerPage = PAGE_SIZE / rowSize;

    /* will be changed when loading the data saved permanently to disk with the number of rows of the stored table */
    this -> numRows = 0;
}

Table::~Table () {
    for (vector<Page*>::iterator itr = this -> pages.begin(); itr != this -> pages.end(); ++itr) {
        delete *itr;
    }

    for (map<string, AbstractDataType*>::iterator itr = this -> datatypes.begin(); itr != this -> datatypes.end(); ++itr) {
        delete itr->second;
    }
}


Page* Table::getPage (uint32_t rowNum) {
    uint32_t pageNum = rowNum / this -> rowsPerPage;
    if (pageNum > TABLE_MAX_PAGE) {
        throw Exceptions::TableSizeExceeded ("Max number of pages for the table reached");
    }
    Page *page = this -> pages[pageNum];
    if (page == nullptr) {
        page = this -> pages[pageNum] = new Page();
    }

    return page;
}

vector<Page*> Table::getPages () {
    return this -> pages;
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

    try {
        // TEMPORARY: numRows will be substituted with the correct number in the b+ tree
        Page *page = this -> getPage(this -> numRows);
        uint32_t rowOffset = this -> numRows % this -> rowsPerPage;
        uint32_t byteOffset = rowOffset * this -> rowSize;
        void *rowSlot = page->getRows() + byteOffset;
        page->serializeRow(row, rowSlot);
    } catch (Exceptions::TableSizeExceeded& e) {
        throw;
    }

}

vector<Row*> Table::getRows () {
    vector<AbstractDataType*> vecDatatypes;
    for (vector<string>::iterator itr = this->attrNames.begin(); itr != this->attrNames.end(); ++itr) {
        vecDatatypes.push_back(this->datatypes[*itr]);
    }
    
    vector<Row*> rows;
    // temporary scan all the rows
    for (uint32_t i = 0; i < this -> numRows; ++i) {
        Row *row = new Row();
        Page *page = this -> getPage(i);
        uint32_t rowOffset = i % this -> rowsPerPage;
        uint32_t byteOffset = rowOffset * this -> rowSize;
        void *rowSlot = page->getRows() + byteOffset;
        page->deserializeRow(rowSlot, row, vecDatatypes);
        rows.push_back(row);
    }
    return rows;
}
