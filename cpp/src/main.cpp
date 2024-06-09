# include <iostream>
# include "Buffer.h"
# include "Statement.h"

using namespace std;

/* REPL to perform operation and to retrieve data on the database */


void printPrompt () {
    cout << "db > ";
}

void readInput (Buffer *inputBuffer) {
    inputBuffer -> setBuffer ();
}

void closeBuffer (Buffer *inputBuffer) {
    delete inputBuffer;
}

void doMetaCommand (Buffer *inputBuffer) {
    if (inputBuffer->getBuffer() == ".exit") {
        exit(EXIT_SUCCESS);
        return;
    }

    throw runtime_error("unrecognized command");
}


int main () {

    Buffer *inputBuffer = new Buffer ();
    /* TO-DO: move exception throwing in the constructor and delete prepareStatement */
    Statement *statement = new Statement ();

    while (true) {
        printPrompt();
        readInput (inputBuffer);

        // no SQL commands
        if (inputBuffer->getBuffer()[0] == '.') {
            try {
                doMetaCommand(inputBuffer);
            } catch (runtime_error& e) {
                cout << "Unrecognized command " << inputBuffer -> getBuffer() << "\n";
            }
        }

        try {
            statement->prepareStatement (inputBuffer);
        } catch (runtime_error& e) {
            cout << "Unrecognized keyword at start of " << inputBuffer->getBuffer() << "\n";
        }

        statement->executeStatement();
        cout << "Executed\n";
    }
}