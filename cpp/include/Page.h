# ifndef PAGE_CLASS
# define PAGE_CLASS

# include "Row.h"
# include <vector>

class Page {
    public:
        Page ();
        ~Page ();
        void* getRows ();
        void setRows (void *rows);
        void serializeRow (Row *source, void *destination);
        void deserializeRow (void *source, Row *destination, vector<AbstractDataType*> datatypes);

    private:
        void *rows;

};

# endif