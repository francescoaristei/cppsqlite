# ifndef EXCEPTIONS_CLASS
# define EXCEPTIONS_CLASS

# include <string>
# include <stdexcept>

using namespace std;

class Exceptions {
    public:
        class UnrecognizedStatement: public runtime_error {
            public:
                UnrecognizedStatement(const string& message) 
                    : message(message)
                    , runtime_error(message) 
                    {}
                string getMessage () {return this -> message;}
            private:
                string message;
        };
        class UnrecognizedMetaCommand: public runtime_error {
            public:
                UnrecognizedMetaCommand(const string& message) 
                    : message(message)
                    , runtime_error(message) 
                    {}
                string getMessage () {return this -> message;}
            private:
                string message;
        };
        class SyntaxError: public runtime_error {
            public:
                SyntaxError(const string& message) 
                    : message(message)
                    , runtime_error(message) 
                    {}
                string getMessage () {return this -> message;}
            private:
                string message;
        };
        class TableSizeExceeded: public runtime_error {
            public:
                TableSizeExceeded(const string& message) 
                    : message(message)
                    , runtime_error(message) 
                    {}
                string getMessage () {return this -> message;}
            private:
                string message;
        };
};

# endif