# include "Buffer.h"
# include "Statement.h"
# include <iostream>
# include <sstream>
# include <vector>
# include <limits>
# include "Uint32_t.h"
# include "Exceptions.h"
# include "VarChar32.h"
# include "VarChar255.h"
# include "Row.h"
# include "Table.h"
# include "utils.h"

using namespace std;

Statement::Statement () {
}

Statement::~Statement () {
    delete rowToInsert;
    delete tableTo;
}

/* the SQL compiler */
void Statement::prepareStatement (Buffer *inputBuffer, Database *database) {
    stringstream stream(inputBuffer->getBuffer());
    vector<AbstractDataType*> tokens;
    string token;
    string command;
    getline(stream, command, ' ');
    /* example: create tablename uint32_t id varchar32 attribute1 varchar32 attribute2 uint32_t attribute3 ... */
    if (command == "create") {
        this -> type = STATEMENT_CREATE;
        getline(stream, token, ' ');
        string tableName = token;
        vector<string> datatypes;
        vector<string> attrNames;
        int counter = 0;
        while (getline(stream, token, ' ')) {
            if (token == "uint32_t" || token == "varchar32" || token == "varchar255") {
                if (counter++ != 0) {
                    throw Exceptions::SyntaxError("Each attribute name has an associated data type.");
                }
                datatypes.push_back(token);
            } else { // attr name
                if (counter-- != 1) {
                    throw Exceptions::SyntaxError("Each attribute name has an associated data type.");
                }
                attrNames.push_back(token);
            }
        }
        if (datatypes.size() != attrNames.size()) {
            // error, must match
            throw Exceptions::SyntaxError("Each attribute name has an associated data type.");
        }

        /* write table details to disk */
        database -> addTable(tableName, datatypes, attrNames);
        return;

    /* example: insert into tablename value_of_attribute1 value_of_attribute2 value_of_attribute3 ... etc */
    } else if (command == "insert") {
        this -> type = STATEMENT_INSERT;
        while (getline(stream, token, ' ')) {
            if (token == "into") {
                // insert into the table with this name
                getline(stream, token, ' ');
                this -> tableTo = database->getTable(token);
            } else {
                if (isANumber(token)) {
                    /* TO-DO: this check is not needed for all attributes, for example is needed for primary key, but not others */
                    if (token[0] != '-') {
                        size_t pos;
                        uint32_t value = stoul(token, &pos, 10);
                        if (pos == token.size() && value >= 0 && value <= numeric_limits<uint32_t>::max()) {
                            tokens.push_back(new Uint32_t(value));
                        }
                        else {
                            throw Exceptions::SyntaxError("Attribute is not a uint32_t.");
                        }
                    } else {
                        throw Exceptions::SyntaxError("Primary key cannot be negative.");
                    }
                }
                 else {
                    if (token.size() <= 32) {
                        if (token.empty()) {
                            throw Exceptions::SyntaxError("Attribute is empty.");
                        }
                        tokens.push_back(new VarChar32(token));
                    } else if (token.size() <= 255) {
                        tokens.push_back(new VarChar255(token));
                    } else {
                        throw Exceptions::SyntaxError("Attribute has not correct size.");
                    }
                }
            }
        }
        
        // TO-DO: check that tokens match the attributes of the row
        this -> rowToInsert = new Row(tokens);
        return;
        /* example: select from tablename */
    } else if (command == "select") {
        this -> type = STATEMENT_SELECT;
        while (getline(stream, token, ' ')) {
            if (token == "from") {
                // insert into the table with this name
                getline(stream, token, ' ');
                this -> tableTo = database-> getTable(token);
            }
        }
        return;
    } else if (command == "exit") {
        this -> type = STATEMENT_CLOSE;
        return;
    }

    throw Exceptions::UnrecognizedStatement("Unrecognized Statement");
}

void Statement::executeCreate (ostream& output) {
    output << "Table schema correctly written to Disk.\n";
}

void Statement::executeSelect (ostream& output) {
    vector<Row*> rows = this -> tableTo-> getRows();
    // FOR NOW, PRINT EVERY ROW
    for (vector<Row*>::iterator itr_rows = rows.begin(); itr_rows != rows.end(); ++itr_rows) {
            (*itr_rows)->printRow(output);
            output << "\n";
    }

    /* free memory from allocated rows */
    for (vector<Row*>::iterator itr_rows = rows.begin(); itr_rows != rows.end(); ++itr_rows) {
        delete *itr_rows;
    }
}

void Statement::executeInsert (ostream& output) {
    try {
        this -> tableTo->insertRow(this -> rowToInsert);
        output << "Executed\n";
    } catch (runtime_error& e) {
        throw;
    }
}

void Statement::executeClose (ostream& output) {
    this -> tableTo -> getPager() -> closeTable(this -> tableTo -> getNumRows());
}

/* the "Virtual Machine" */
void Statement::executeStatement (ostream& output) {
    switch (this->type) {
        case StatementType::STATEMENT_CREATE:
            executeCreate(output);
            break;
        case StatementType::STATEMENT_INSERT:
            executeInsert(output);
            break;
        case StatementType::STATEMENT_SELECT:
            executeSelect(output);
            break;
        case StatementType::STATEMENT_CLOSE:
            executeClose(output);
            break;
    }
}