# ifndef VARCHAR32_CLASS
# define VARCHAR32_CLASS

# include "AbstractDataType.h"
# include <string>
# include <any>

using namespace std;

class VarChar32: public AbstractDataType {
    public:
        VarChar32 (const string& value = "default");
        any getValue ();
        string toString ();
        uint32_t getSize ();

    private:
        string value;
        uint32_t size;

};

# endif