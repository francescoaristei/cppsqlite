# ifndef DATABASE_CLASS
# define DATABASE_CLASS

# include "Table.h"
# include <string>
# include <map>
# include <vector>


class Database {
    public:
        Database ();
        ~Database ();
        Table* getTable (string name);
        vector<Table*> getTables ();
        void addTable (string name, vector<string> datatypes, vector<string> attributeNames);

    private:
        map<string, Table*> tables;
};


# endif