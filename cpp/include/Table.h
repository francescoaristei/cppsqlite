# ifndef TABLE_CLASS
# define TABLE_CLASS

# include <vector>
# include <map>
# include "Page.h"
# include "Row.h"
# include "AbstractDataType.h"
# include "Pager.h"

using namespace std;

class Table {
    public:
        Table (map<string, AbstractDataType*> datatypes, vector<string> attrNames, string name);
        ~Table ();
        vector<Page*> getPages ();
        map<string, AbstractDataType*> getDatatypes ();
        //Page* getPage (uint32_t rowNum);
        void setNumRows ();
        void insertRow (Row *row);
        vector<Row*> getRows ();
        // temporary
        uint32_t getNumRows ();
        Pager* getPager ();

    private:
        //uint32_t rowsPerPage;
        //uint32_t rowSize;
        uint32_t numRows;
        Pager *pager;
        //vector<Page*> pages;
        vector<string> attrNames;
        map<string, AbstractDataType*> datatypes;

};

# endif