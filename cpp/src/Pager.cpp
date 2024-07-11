# include "Pager.h"
# include "utils.h"
# include "Exceptions.h"
# include <string>
# include <iostream>
# include <fstream>
# include <map>
# include <vector>
# include <any>

/* 
PROBLEMS:
- The number of rows written is incorrect. It is written a number of rows = numPages * rowsPerPage. Should be written the number
   of rows actually inserted.
*/

/*
TO-DEBUG:

To check the content of the binary file: cat cpp/src/database/table1.txt | hexdump -C
Probably the problem is in the flushPage method, Should it resamble the serializeRow method?

Moreover, the way the serializeRow method writes in the void* array could be wrong now that the page is loaded from file:
is the numRows correct now? Is taking into consideration the rows already in the page, loaded from the file storing the table?
*/

using namespace std;


Pager::Pager (string fileName, uint32_t rowSize, map<string, AbstractDataType*> datatypes) : pages(TABLE_MAX_PAGE) {
    ifstream fileReader(fileName, ios::binary | ios::ate); /* ios::ate to move to the end of the file */
    if (!fileReader.is_open()) {
        cout << "Error opening the table file.\n";
        return;
    }
    this -> fileLength = fileReader.tellg();
    this -> fileName = fileName;
    this -> datatypes = datatypes;
    for (uint32_t i = 0; i < TABLE_MAX_PAGE; i++) {
        this -> pages[i] = nullptr;
    }
    this -> rowSize = rowSize;
    this -> numOfRows = fileLength / this -> rowSize;
    this -> rowsPerPage = PAGE_SIZE / this -> rowSize;
    /* if numPages = 1 then the this -> pages array has the entry with index 0 */
    this -> numPages = (this -> fileLength + PAGE_SIZE - 1) / PAGE_SIZE; /* ceil division */
    fileReader.close();
}

Pager::~Pager () {
    for (vector<Page*>::iterator itr = this -> pages.begin(); itr != this -> pages.end(); ++itr) {
        delete *itr;
    }

    for (map<string, AbstractDataType*>::iterator itr = this -> datatypes.begin(); itr != this -> datatypes.end(); ++itr) {
        delete itr->second;
    }
}

fstream::pos_type Pager::getFileLength () {
    return this -> fileLength;
}

vector<Page*> Pager::getPages () {
    return this -> pages;
}

Page* Pager::getPage (uint32_t rowNum) {
    uint32_t pageNum = rowNum / this -> rowsPerPage;
    if (pageNum > TABLE_MAX_PAGE) {
            throw Exceptions::TableSizeExceeded ("Max number of pages for the table reached");
    }

    /* caching mechanism */
    Page *page = this -> pages[pageNum];
    if (page == nullptr) {
        page = this -> pages[pageNum] = new Page();
        //uint32_t numPages = this -> fileLength / PAGE_SIZE;

        /* save partial page at the end of file */
        if (this -> fileLength % PAGE_SIZE) {
            this -> numPages += 1;
        }

        if (pageNum <= numPages) {
            ifstream fileReader(this -> fileName, ios::binary);
            if (!fileReader.is_open()) {
                cout << "Error opening the table file to get the page.\n";
            }
            /* set the rows of the page with the rows read from the table file */
            void *rows = new void*[PAGE_SIZE];
            fileReader.seekg(pageNum * PAGE_SIZE);
            if (!fileReader) {
                cout << "Failed to seek to the correct position in the file.\n";
                // Handle error, clean up, and return if necessary
            }
            fileReader.read(reinterpret_cast<char*>(rows), PAGE_SIZE);
            page -> setRows(rows);
            fileReader.close();
        }
    }

    return this -> pages[pageNum];
}


void Pager::insertRow (Row *row, uint32_t numRows) {
    try {
        Page *page = this -> getPage(numRows);
        uint32_t rowOffset = numRows % this -> rowsPerPage;
        uint32_t byteOffset = rowOffset * this -> rowSize;
        void *rowSlot = page->getRows() + byteOffset;
        page->serializeRow(row, rowSlot);
        this -> numOfRows += 1;
    } catch (Exceptions::TableSizeExceeded& e) {
        throw;
    }
}


vector<Row*> Pager::getRows (vector<string> attrNames, uint32_t numRows) {
    vector<AbstractDataType*> vecDatatypes;
    for (vector<string>::iterator itr = attrNames.begin(); itr != attrNames.end(); ++itr) {
        vecDatatypes.push_back(this->datatypes[*itr]);
    }
    vector<Row*> rows;
    for (uint32_t i = 0; i < numRows; ++i) {
        Row *row = new Row();
        Page *page = this -> getPage(i);
        uint32_t rowOffset = i % this -> rowsPerPage;
        uint32_t byteOffset = rowOffset * this -> rowSize;
        void *rowSlot = page -> getRows() + byteOffset;
        page -> deserializeRow(rowSlot, row, vecDatatypes);
        rows.push_back(row);
    }
    return rows;
}

uint32_t Pager::getNumRows () {
    //return this -> numPages * this -> rowsPerPage;
    return this -> numOfRows;
}


void Pager::closeTable (uint32_t numRows) {
    uint32_t numFullPages = numRows / this -> rowsPerPage;
    if (!numFullPages) { /* if only 1 page (and not full) */
        if (this -> pages[0] != nullptr) {
            //this -> flushPage(0, PAGE_SIZE);
            this -> flushPage(0, this -> rowSize * this -> numOfRows);
            this -> pages[0] = nullptr;
        }
    } else {
        for (uint32_t i = 0; i < numFullPages; i++) {
            if (this -> pages[i] == nullptr) {
                continue;
            }
            //this -> flushPage(i, PAGE_SIZE);
            this -> flushPage(i, this -> rowSize * this -> numOfRows);
            this -> pages[i] = nullptr;
        }
    }

    /* partial page to write to the end of the file */
    if (numOfRows > this -> rowsPerPage) { /* only if number of rows > number of rows in one page to have meaningful remainder */
        uint32_t numAdditionalRows = numRows % this -> rowsPerPage;
        if (numAdditionalRows > 0) {
            uint32_t pageNum = numFullPages;
            if (this -> pages[pageNum] != nullptr) {
                flushPage(pageNum, numAdditionalRows * this -> rowSize);
                this -> pages[pageNum] = nullptr;
            }
        }
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGE; i++) {
        if (this -> pages[i] != nullptr) {
            this -> pages[i] = nullptr;
        }
    }
}


void Pager::flushPage (uint32_t pageNum, uint32_t size) {
    if (this -> pages[pageNum] == nullptr) {
        cout << "Tried to flush null page\n";
        return;
    }
    ofstream fileWriter(this -> fileName, ios::binary);
    if (!fileWriter.is_open()) {
        cout << "Error opening the file to flush the pages.\n";
        return;
    }
    vector<AbstractDataType*> datatypesVec;
    for (map<string, AbstractDataType*>::iterator itr = this -> datatypes.begin(); itr != this -> datatypes.end(); ++itr) {
        datatypesVec.push_back(itr -> second);
    }
    /* position on the file */
    fileWriter.seekp(pageNum * PAGE_SIZE);
    /* starting offset */
    uint32_t offset = 0;
    /* to understand when to stop */
    uint32_t overallSize = 0;
    while (overallSize < size) {
        for (vector<AbstractDataType*>::iterator itr = datatypesVec.begin(); itr != datatypesVec.end(); ++itr) {
            any value = (*itr)->getValue();
            uint32_t attrSize = (*itr)->getSize();
            if (value.type() == typeid(uint32_t)) {
                fileWriter.write(reinterpret_cast<char*>(this -> pages[pageNum] -> getRows() + offset), attrSize);
                offset += attrSize;
            }
            else if (value.type() == typeid(string)) {
                fileWriter.write(reinterpret_cast<char*>(this -> pages[pageNum] -> getRows() + offset), attrSize);
                offset += attrSize;
            }
            overallSize += attrSize;
        }
    }
    fileWriter.close();
}
