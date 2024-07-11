# include "Table.h"
# include "Database.h"
# include "AbstractDataType.h"
# include "Uint32_t.h"
# include "VarChar255.h"
# include "VarChar32.h"
# include <string>
# include <map>
# include <vector>
# include <fstream>
# include <iostream>
# include <sstream>

using namespace std;

Database::Database () {
    ifstream fileReader("cpp/src/dbfiles/dbinfo.txt");
    if (!fileReader.is_open()) {
        cout << "Error opening the dbinfo file.\n";
        return;
    }

    string line;
    while (getline(fileReader, line)) {
        map<string, AbstractDataType*> attrToDt;
        string name;
        vector<string> datatypes;
        vector<string> attributeNames;

        size_t openBracket = line.find('(');
        size_t closedBracket = line.find(')');

        name = line.substr(0, openBracket);

        string datatypesString = line.substr(openBracket + 1, closedBracket - openBracket);
        stringstream datatypesStream(datatypesString);
        string datatype;
        while (getline(datatypesStream, datatype, ','))
            datatypes.push_back(datatype);

        string attributeNamesString = line.substr(closedBracket + 1, line.size() - openBracket);
        stringstream attributeNamesStream(attributeNamesString);
        string attributeName;
        while (getline(attributeNamesStream, attributeName, ','))
            attributeNames.push_back(attributeName);

        vector<string>::iterator datatypesItr = datatypes.begin();
        vector<string>::iterator attributeNamesItr = attributeNames.begin();
        for (;datatypesItr != datatypes.end(), attributeNamesItr != attributeNames.end(); ++datatypesItr, ++attributeNamesItr) {
            if (*datatypesItr == "uint32_t") {
                attrToDt[*attributeNamesItr] = new Uint32_t();
            } else if (*datatypesItr == "varchar32") {
                attrToDt[*attributeNamesItr] = new VarChar32();
            } else {
                attrToDt[*attributeNamesItr] = new VarChar255();
            }
        }
        this -> tables[name] = new Table(attrToDt, attributeNames, name);   
    }

    fileReader.close();
}

Database::~Database () {
    for (map<string, Table*>::iterator itr = tables.begin(); itr != tables.end(); ++itr) {
        delete itr->second;
    }
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

/* internal format of table on dbinfo line: name(uint32_t,varchar255,uint32_t...etc)attribute1,attribute2,attribute3....etc */
void Database::addTable (string name, vector<string> datatypes, vector<string> attributeNames) {

    ofstream fileWriter("cpp/src/dbfiles/dbinfo.txt", ios::app);
    if (!fileWriter.is_open()) {
        cout << "Error opening the file.\n";
        return;
    }

    fileWriter << name;
    fileWriter << "(";

    for (vector<string>::iterator itr = datatypes.begin(); itr != datatypes.end(); ++itr) {
        if (itr != datatypes.end() - 1) {
            fileWriter << *itr;
            fileWriter << ",";
        } else {
            fileWriter << *itr;
            fileWriter << ")";
        }
    }

    for (vector<string>::iterator itr = attributeNames.begin(); itr != attributeNames.end(); ++itr) {
        if (itr != attributeNames.end() - 1) {
            fileWriter << *itr;
            fileWriter << ",";
        } else {
            fileWriter << *itr;
            fileWriter << "\n";
        }
    }
    fileWriter.close();

    /* create the table file upfront, so the pager does not have to worry about it */
    ofstream tableWriter("cpp/src/database/" + name + ".txt");
    if (!tableWriter.is_open()) {
        cout << "Error creating the table file.\n";
        return;
    }
    tableWriter.close();
}
