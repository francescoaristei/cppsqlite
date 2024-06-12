# ifndef UINT32_T_CLASS
# define UINT32_T_CLASS

# include "AbstractDataType.h"
# include <stdint.h>
# include <any>

using namespace std;

class Uint32_t: public AbstractDataType {
    public:
        Uint32_t (uint32_t value = 0);
        any getValue ();
        string toString ();
        uint32_t getSize ();

    private:
        uint32_t value;
        uint32_t size;

};

# endif