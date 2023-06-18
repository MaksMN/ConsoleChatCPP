#include "NetworkOptions.h"

char NetworkOptions::remove(char value, sv::options option)
{
    return (char)f.removeFlag((sv::options)value, option);
}

bool NetworkOptions::is(char data, sv::options option)
{
    return f.hasFlag(data, option);
}