# ifndef PAGE_CLASS
# define PAGE_CLASS

# include "Row.h"
# include <vector>

class Page {
    public:
        Page ();
        void* getRows ();
        void serializeRow (Row *source, void *destination);
        void deserializeRow (void *source, Row *destination, vector<AbstractDataType*> datatypes);

    private:
        void *rows;

};

# endif