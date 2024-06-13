# ifndef BUFFER_CLASS
# define BUFFER_CLASS

# include <string>

using namespace std;


class Buffer {
    public:
        Buffer ();
        ~Buffer ();
        void setBuffer ();
        void setBuffer (string s);
        string getBuffer ();
        size_t getBufferLength ();
        ssize_t getInputLength ();
    private:
        string buffer;
        size_t buffer_length;
        ssize_t input_length;
};

# endif