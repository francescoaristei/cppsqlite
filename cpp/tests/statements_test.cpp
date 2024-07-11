# define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
# include "doctest.h"
# include "Statement.h"
# include <string>
# include "Exceptions.h"

// TEMPORARY
# include "AbstractDataType.h"
# include "VarChar255.h"
# include "VarChar32.h"
# include "Uint32_t.h"

using namespace std;

/* ---------TEMPORARY---------- */
/* Table should be extracted from the Database object storing all the tables */
map<string, AbstractDataType*> datatypes;
vector<string> attrNames;
Table *tableTo;
Buffer *inputBuffer;
Statement *statement;
/* TO-DO: create object from tables extracted from disk */
Database *database = new Database ();

void initialize_data () {
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
}

void initialize_query(string query) {
    inputBuffer -> setBuffer(query);
}
/* ------------------------------- */


TEST_CASE("Testing insert statement") {
    string query = "insert 1 person1 person1@examle.com";
    stringstream ss;
    initialize_data();

    // temporary
    inputBuffer = new Buffer();
    initialize_query(query);
    tableTo = new Table(datatypes, attrNames);
    statement = new Statement(tableTo);

    statement->prepareStatement (inputBuffer, database);
    statement->executeStatement(ss);
    CHECK(ss.str() == "Executed\n");

}

/* to modify */
TEST_CASE("Testing making the table full") {
    string query;
    stringstream ss;
    initialize_data();

    tableTo = new Table(datatypes, attrNames);
    statement = new Statement(tableTo);

    try {
        for (int i = 0; i < 1401; i++) {
            query = "insert " + to_string(i) + " person" + to_string(i) + " person" + to_string(i) + "@example.com";

            inputBuffer = new Buffer();
            initialize_query(query);

            statement->prepareStatement(inputBuffer, database);
            statement->executeStatement(ss);
        }
    } catch (Exceptions::TableSizeExceeded& e) {
            CHECK(e.getMessage() == "Max number of pages for the table reached");
    }
}


TEST_CASE("Testing inserting a negative ID") {
    string query = "insert -1 person1 person1@example.com";
    stringstream ss;

    // temporary
    initialize_data();
    inputBuffer = new Buffer();
    initialize_query(query);
    tableTo = new Table(datatypes, attrNames);
    statement = new Statement(tableTo);

    DOCTEST_CHECK_THROWS_AS(statement->prepareStatement(inputBuffer, database), Exceptions::SyntaxError);
}