# include <iostream>
# include "Buffer.h"
# include "Statement.h"
# include "Uint32_t.h"
# include "VarChar255.h"
# include "Exceptions.h"
# include "VarChar32.h"

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

    throw Exceptions::UnrecognizedMetaCommand("Unrecognized command");
}


int main () {

    Buffer *inputBuffer = new Buffer ();
    Statement *statement = new Statement ();
    Database *database = new Database ();

    while (true) {
        printPrompt();
        readInput (inputBuffer);

        // no SQL commands
        if (inputBuffer->getBuffer()[0] == '.') {
            try {
                doMetaCommand(inputBuffer);
            } catch (Exceptions::UnrecognizedMetaCommand& e) {
                cout << e.getMessage() << "\n";
            }
        }
        try {
            statement->prepareStatement (inputBuffer, database);
            statement->executeStatement(cout);
        } catch (Exceptions::UnrecognizedStatement& e) {
            cout << e.getMessage() << "\n";
        } catch (Exceptions::SyntaxError& e) {
            cout << e.getMessage() << "\n";
        } catch (Exceptions::TableSizeExceeded& e) {
            cout << e.getMessage() << "\n";
        }
    }

    /* free memory */
    delete inputBuffer;
    delete statement;
    delete database;
}