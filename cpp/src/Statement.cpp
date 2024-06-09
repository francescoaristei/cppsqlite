# include "Buffer.h"
# include "Statement.h"
# include <iostream>

using namespace std;

Statement::Statement () {

}

Statement::~Statement () {
}

/* the SQL compiler */
void Statement::prepareStatement (Buffer *inputBuffer) {
    if (inputBuffer->getBuffer().substr(0, 6) == "insert") {
        this -> type = STATEMENT_INSERT;
        return;
    }
    if (inputBuffer->getBuffer().substr(0, 6) == "select") {
        this -> type = STATEMENT_SELECT;
        return;
    }

    throw runtime_error("unrecognized statement");
}

/* the "Virtual Machine" */
void Statement::executeStatement () {
    switch (this->type) {
        case StatementType::STATEMENT_INSERT:
            cout << "Where the insert is handled\n";
            break;

        case StatementType::STATEMENT_SELECT:
            cout << "Where the select in handled\n";
            break;
    }
}