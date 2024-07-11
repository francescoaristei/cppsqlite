# ifndef STATEMENT_CLASS
# define STATEMENT_CLASS

# include "Buffer.h"
# include "Row.h"
# include "Table.h"
# include "Database.h"
# include <vector>
# include <stdexcept>

using namespace std;

enum StatementType {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_CREATE,
    STATEMENT_CLOSE
};

class Statement {
    public:
        Statement (/*Table *tableTo*/);
        ~Statement ();
        void prepareStatement (Buffer *inputBuffer, Database *database);
        void executeStatement (ostream& output);
        StatementType getType ();
    
    private:
        void executeInsert (ostream& output);
        void executeSelect (ostream& output);
        void executeCreate (ostream& output);
        void executeClose (ostream& output);
        StatementType type;
        Row *rowToInsert;
        Table *tableTo;
        Database *database;

};

# endif