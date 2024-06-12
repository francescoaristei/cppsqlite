# include "Table.h"
# include "Database.h"
# include <string>
# include <map>
# include <vector>


Database::Database () {

}

Database::~Database () {

}

Table* Database::getTable (string name) {
    return this -> tables[name];
}

vector<Table*> Database::getTables () {
    vector<Table*> toreturn;
    for (map<string, Table*>::iterator itr = tables.begin(); itr != tables.end(); ++itr) {
        toreturn.push_back(itr->second);
    }
    return toreturn;
}

void Database::addTable (string name, Table *table) {
    this -> tables[name] = table;
}
