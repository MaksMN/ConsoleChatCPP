#pragma once
#include <string>
#include <iostream>
#include "../../Misc/Misc.h"
const extern char config_file[];
class SQL
{
protected:
    const std::string server;
    const std::string port;
    const std::string dbuser;
    const std::string dbpass;
    const std::string odbcdriver;

public:
    SQL();
    virtual ~SQL() = default;
    virtual void hello() = 0;
};