# include <string>
# include <iostream>
using namespace std;


bool isANumber (const string& token) {
    string::const_iterator it = token.begin();
    if (token.empty())
        return false;
    
    int dots = 0;
    while (it != token.end()) {
        if (!isdigit(*it)) {
            if (it == token.begin()) {
                if (*it != '-') {
                    return false;
                }
            } else {
                if (it == token.end()) {
                    return false;
                } else {
                    if (*it != '.') {
                        return false;
                    } else {
                        if (dots) {
                            return false;
                        }
                        dots += 1;
                    }
                }
            }
        }
        ++it;
    }
    return true;
}