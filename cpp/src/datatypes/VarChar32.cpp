# include "VarChar32.h"
# include <string>
# include <any>

using namespace std;


VarChar32::VarChar32 (const string& value) {
    this -> value = value;
    this -> size = 32;
}

any VarChar32::getValue () {
    return this -> value;
}

uint32_t VarChar32::getSize () {
    return this -> size;
}

string VarChar32::toString () {
    return this -> value;
}