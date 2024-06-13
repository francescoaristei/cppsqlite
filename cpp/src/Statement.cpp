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

/* TEMPORARY: tableTo is set in the prepareStatement, obtained from database, reading the table of the query */
Statement::Statement (Table *tableTo) {
    this -> tableTo = tableTo;

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
    if (command == "insert") {
        while (getline(stream, token, ' ')) {
            this -> type = STATEMENT_INSERT;
            if (token == "into") {
                // insert into the table with this name
                getline(stream, token, ' ');
                // something like 
                // tableTo = database.getTable(token);
                
            } else {
                if (isANumber(token)) {
                    size_t pos;
                    uint32_t value = stoul(token, &pos, 10);
                    if (pos == token.size() && value <= numeric_limits<uint32_t>::max()) {
                        tokens.push_back(new Uint32_t(value));
                    }
                    else {
                        throw Exceptions::SyntaxError("Attribute is not a uint32_t");
                    }
                }
                 else {
                    if (token.size() <= 32) {
                        tokens.push_back(new VarChar32(token));
                    } else {
                        tokens.push_back(new VarChar255(token));
                    }
                }
            }
        }
        
        this -> rowToInsert = new Row(tokens);
        return;
    }
    if (command == "select") {
        this -> type = STATEMENT_SELECT;
        return;
    }

    throw Exceptions::UnrecognizedStatement("Unrecognized Statement");
}

void Statement::executeSelect (ostream& output) {
    vector<Row*> rows = tableTo->getRows();
    // FOR NOW, PRINT EVERY ROW
    for (vector<Row*>::iterator itr_rows = rows.begin(); itr_rows != rows.end(); ++itr_rows) {
            (*itr_rows)->printRow(output);
            output << "\n";
            //cout << "\n";
    }

    /* free memory from allocated rows */
    for (vector<Row*>::iterator itr_rows = rows.begin(); itr_rows != rows.end(); ++itr_rows) {
        delete *itr_rows;
    }
}

void Statement::executeInsert (ostream& output) {
    try {
        tableTo->insertRow(this -> rowToInsert);
        tableTo->setNumRows();
        output << "Executed\n";
    } catch (runtime_error& e) {
        throw;
    }
}

/* the "Virtual Machine" */
void Statement::executeStatement (ostream& output) {
    switch (this->type) {
        case StatementType::STATEMENT_INSERT:
            executeInsert(output);
            break;
        case StatementType::STATEMENT_SELECT:
            executeSelect(output);
            break;
    }
}