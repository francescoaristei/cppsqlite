    
# include <string>
# include <iostream>
# include "Buffer.h"

using namespace std;

Buffer::Buffer () {
    
}


Buffer:: ~Buffer () {

}

void Buffer::setBuffer () {
    getline(cin, this->buffer);
}

string Buffer::getBuffer () {
    return this -> buffer;
}

size_t Buffer::getBufferLength () {

}

ssize_t Buffer::getInputLength () {

}
