# include <string>

using namespace std;


bool isANumber (const string& token) {
    string::const_iterator it = token.begin();
    while (it != token.end()) {
        if (!isdigit(*it))
            return false;
        ++it;
    }
    return true;
}