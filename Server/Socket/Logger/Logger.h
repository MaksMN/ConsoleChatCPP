#include <thread>
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include "../../../Misc/BufferActions.h"

class Logger
{
private:
    std::mutex mutex;
    std::string log_file_name = "access.log";
    std::fstream file_stream;

public:
    Logger();
    ~Logger();

    void write(char (&_cmd_buffer)[CMD_BUFFER]);
    std::string read();
};