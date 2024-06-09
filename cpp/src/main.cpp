# include <iostream>
# include "Buffer.h"

using namespace std;

/* REPL to perform operation and to retrieve data on the database */

void print_prompt () {
    cout << "db > ";
}

void read_input (Buffer *inputBuffer) {
    inputBuffer -> setBuffer ();
}

void close_buffer (Buffer *inputBuffer) {
    delete inputBuffer;
}


int main () {

    Buffer *inputBuffer = new Buffer ();

    while (true) {
        print_prompt();
        read_input (inputBuffer);

        if (inputBuffer->getBuffer() == ".exit") {
            close_buffer (inputBuffer);
            exit(EXIT_SUCCESS);
        }
        else {
            cout << "Unrecognized command " << inputBuffer->getBuffer();
        }
    }
}