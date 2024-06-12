# include "Page.h"
# include "Row.h"
# include "Uint32_t.h"
# include "VarChar32.h"
# include "VarChar255.h"
# include "utils.h"
# include <vector>
# include <string>
# include <cstring>
# include <any>

# include <iostream>

using namespace std;


Page::Page () {
    this -> rows = new void*[PAGE_SIZE];
}

void Page::serializeRow (Row *source, void *destination) {
    vector<uint32_t> rowSizes = source -> getSizes();
    vector<uint32_t> rowOffsets = source -> getOffsets();
    vector<AbstractDataType*> attrs = source->getAttributes();

    vector<uint32_t>::iterator itrSizes = rowSizes.begin();
    vector<uint32_t>::iterator itrOff = rowOffsets.begin();
    vector<AbstractDataType*>::iterator itrRow = attrs.begin();

    try {
        for (;itrSizes != rowSizes.end(), itrOff != rowOffsets.end(), itrRow != attrs.end(); ++itrOff, ++itrSizes, ++itrRow) {
            any value = (*itrRow)->getValue();
            if (value.type() == typeid(uint32_t)) {
                uint32_t concreteValue = any_cast<uint32_t>(value);
                memcpy (destination + *itrOff, &(concreteValue), *itrSizes);
            }
            else if (value.type() == typeid(string)) {
                string concreteValue = any_cast<string>(value);
                memcpy (destination + *itrOff, concreteValue.c_str(), *itrSizes + 1); // + 1 for null terminator
            } 
        }
    } catch (const bad_any_cast& e) {
        cout << e.what() << "\n";
    }
}

void Page::deserializeRow (void *source, Row *destination, vector<AbstractDataType*> datatypes) {

    // starting offset
    uint32_t offset = 0;

    for (vector<AbstractDataType*>::iterator itr = datatypes.begin(); itr != datatypes.end(); ++itr) {
        any value = (*itr)->getValue();
        uint32_t size = (*itr)->getSize();
        if (value.type() == typeid(uint32_t)) {
            uint32_t concreteValue;
            memcpy (&concreteValue, source + offset, size);
            Uint32_t *attr = new Uint32_t(concreteValue);
            destination->addAttribute(attr);
            destination->addOffset(offset);
            destination->addSize(attr->getSize());
            offset += attr -> getSize();
        }
        else if (value.type() == typeid(string)) {
            char *concreteValue = new char[size];
            memcpy (concreteValue, source + offset, size);
            string str(concreteValue);
            if (size == 32) {
                VarChar32 *attr = new VarChar32(concreteValue);
                destination->addAttribute(attr);
                destination->addOffset(offset);
                destination->addSize(attr->getSize());
                offset += attr -> getSize();
            } else {
                VarChar255 *attr = new VarChar255(concreteValue);
                destination->addAttribute(attr);
                destination->addOffset(offset);
                destination->addSize(attr->getSize());
                offset += attr -> getSize();
            }
        }
    }
}

void* Page::getRows () {
    return this -> rows;
}
