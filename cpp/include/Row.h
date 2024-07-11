# ifndef ROW_CLASS
# define ROW_CLASS

# include <string>
# include <vector>
# include "AbstractDataType.h"


class Row {
    public:
        Row (vector<AbstractDataType*> attributes);
        Row ();
        ~Row ();
        vector<AbstractDataType*> getAttributes ();
        vector<uint32_t> getSizes ();
        vector<uint32_t> getOffsets ();
        void addAttribute (AbstractDataType *attribute);
        void addOffset (uint32_t offset);
        void addSize (uint32_t size);
        void printRow (ostream& output);
    
    private:
        vector<AbstractDataType*> attributes;
        vector<uint32_t> sizes;
        vector<uint32_t> offsets;
};

using namespace std;

# endif