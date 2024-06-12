# ifndef VARCHAR255_CLASS
# define VARCHAR255_CLASS

# include "AbstractDataType.h"
# include <string>
# include <any>

using namespace std;

class VarChar255: public AbstractDataType {
    public:
        VarChar255 (const string& value = "default");
        any getValue ();
        string toString ();
        uint32_t getSize ();

    private:
        string value;
        uint32_t size;

};

# endif