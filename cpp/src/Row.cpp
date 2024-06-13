# include "Row.h"
# include "AbstractDataType.h"
# include <string>
# include <iostream>
# include <any>


using namespace std;

Row::Row (vector<AbstractDataType*> attributes) {
    this -> attributes = attributes;
    try {
        for (vector<AbstractDataType*>::iterator itr = attributes.begin(); itr != attributes.end(); ++itr) {
            any value = (*itr)->getValue();
            if (value.type() == typeid(uint32_t)) {
                uint32_t concreteValue = any_cast<uint32_t>(value);
                if (this ->offsets.empty()) {
                    this -> offsets.push_back(0);
                }
                else {
                    uint32_t previousSize = this -> sizes.back();
                    uint32_t previousOffset = this -> offsets.back();
                    this -> offsets.push_back(previousOffset + previousSize);
                }
                this -> sizes.push_back(sizeof(concreteValue));
            } else if (value.type() == typeid(string)) {
                string concreteValue = any_cast<string>(value);
                if (this ->offsets.empty()) {
                    this -> offsets.push_back(0);
                }
                else {
                    uint32_t previousSize = this -> sizes.back();
                    uint32_t previousOffset = this -> offsets.back();
                    this -> offsets.push_back(previousOffset + previousSize);
                }
                this -> sizes.push_back(sizeof(concreteValue));
            }
        }
    } catch (const bad_any_cast& e) {
        cout << e.what() << "\n";
    }
}


Row::Row () {

}


Row::~Row () {
    for (vector<AbstractDataType*>::iterator itr = this -> attributes.begin(); itr != this->attributes.end(); ++itr) {
        delete *itr;
    }
}

vector<AbstractDataType*> Row::getAttributes () {
    return this -> attributes;
}

vector<uint32_t> Row::getSizes () {
    return this -> sizes;
}

vector<uint32_t> Row::getOffsets () {
    return this -> offsets;
}

void Row::addAttribute (AbstractDataType *attribute) {
    this -> attributes.push_back(attribute);

}

void Row::addOffset (uint32_t offset) {
    this -> offsets.push_back(offset);
}

void Row::addSize (uint32_t size) {
    this -> sizes.push_back(size);
}

void Row::printRow (ostream& output) {
    //cout << "[";
    output << "[";
    for (vector<AbstractDataType*>::iterator itr = this -> attributes.begin(); itr != this -> attributes.end(); ++itr) {
        if (itr + 1 == this -> attributes.end())
            //cout << (*itr)->toString();
            output << (*itr)->toString();
        else
            //cout << (*itr)->toString() << ", ";
            output << (*itr)->toString() << ", ";
    }
    output << "]";
    //cout << "]";
}
