#pragma once
#include <string>
#include <iostream>
#include "../../Misc/Misc.h"
const extern char config_file[];
class SQL
{
private:
protected:
    std::string server;
    std::string port;
    std::string dbuser;
    std::string dbpass;
    std::string odbcdriver;

public:
    SQL();
    virtual ~SQL() = default;
    virtual void hello() = 0;
};