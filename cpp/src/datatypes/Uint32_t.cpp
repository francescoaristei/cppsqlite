# include "Uint32_t.h"
# include <any>
# include <string>


using namespace std;


Uint32_t::Uint32_t (uint32_t value) {
    this ->value = value;
    this -> size = 4;
}

any Uint32_t::getValue () {
    return this -> value;
}

uint32_t Uint32_t::getSize () {
    return this -> size;
}

string Uint32_t::toString () {
    return to_string(this->value);
}