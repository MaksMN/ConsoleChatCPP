#include "Logger.h"

Logger::Logger()
{
    file_stream.open(log_file_name);
}

Logger::~Logger()
{
    if (file_stream.is_open())
        file_stream.close();
}

void Logger::write(char (&_cmd_buffer)[CMD_BUFFER])
{
    char cmd_buffer[CMD_BUFFER];
    for (int i{0}; i < CMD_BUFFER; i++)
        cmd_buffer[i] = _cmd_buffer[i];
}

dstd::string Logger::read()
{
    return std::string();
}
