# ifndef PAGER_CLASS
# define PAGER_CLASS

# include "Page.h"
# include "Row.h"
# include <stdint.h>
# include <map>
# include <fstream>

class Pager {
    public:
        Pager (string fileName, uint32_t rowSize, map<string, AbstractDataType*> datatypes);
        ~Pager ();
        fstream::pos_type getFileLength ();
        vector<Page*> getPages ();
        Page* getPage(uint32_t rowNum);
        void insertRow (Row *row, uint32_t numRows);
        vector<Row*> getRows (vector<string> attrNames, uint32_t numRows);
        void closeTable (uint32_t numRows);
        void flushPage (uint32_t pageNum, uint32_t size);
        uint32_t getNumRows ();
    private:
        map<string, AbstractDataType*> datatypes;
        uint32_t rowsPerPage;
        uint32_t rowSize;
        uint32_t numOfRows;
        uint32_t numPages;
        string fileName;
        fstream fileTable;
        ifstream::pos_type fileLength;
        vector<Page*> pages;
};

# endif