# include "VarChar255.h"
# include <string>
# include <any>

using namespace std;


VarChar255::VarChar255 (const string& value) {
    this -> value = value;
    this -> size = 255;
}

any VarChar255::getValue () {
    return this -> value;
}

uint32_t VarChar255::getSize () {
    return this -> size;
}

string VarChar255::toString () {
    return this -> value;
}