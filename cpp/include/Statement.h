# ifndef STATEMENT_CLASS
# define STATEMENT_CLASS

# include "Buffer.h"

enum StatementType {
    STATEMENT_INSERT,
    STATEMENT_SELECT
};

class Statement {
    public:
        Statement ();
        ~Statement ();
        void prepareStatement (Buffer *inputBuffer);
        void executeStatement ();
        StatementType getType ();
    
    private:
        StatementType type;

};

# endif