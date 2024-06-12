# ifndef ABSTRACT_DATA_TYPE_CLASS
# define ABSTRACT_DATA_TYPE_CLASS

# include <any>
# include <string>

using namespace std;

class AbstractDataType {
    public:
        virtual any getValue() = 0;
        virtual string toString () = 0;
        virtual uint32_t getSize () = 0;
};


# endif