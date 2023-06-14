typedef unsigned int uint;
class Handler
{
private:
    char (&_buffer)[4096];

public:
    Handler(char (&buffer)[4096]);
    ~Handler() = default;
};