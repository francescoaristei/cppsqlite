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

    /* ---------TEMPORARY---------- */
    /* Table should be extracted from the Database object storing all the tables */
    /* Specifically, will have database reading dbinfo from dbinfo.txt and Pager reading raw pages (set of rows) from various file, one for each table */
    /*map<string, AbstractDataType*> datatypes;
    vector<string> attrNames;
    string attr1 = "id";
    string attr2 = "username";
    string attr3 = "email";
    attrNames.push_back(attr1);
    attrNames.push_back(attr2);
    attrNames.push_back(attr3);
    Uint32_t *id = new Uint32_t();
    VarChar32 *username = new VarChar32();
    VarChar32 *email = new VarChar32();
    datatypes[attr1] = id;
    datatypes[attr2] = username;
    datatypes[attr3] = email;
    Table *tableTo = new Table (datatypes, attrNames);/*
    /* ------------------------------- */

    Buffer *inputBuffer = new Buffer ();

    //Statement *statement = new Statement (tableTo);
    Statement *statement = new Statement ();

    /* TO-DO: create object from tables extracted from disk */
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