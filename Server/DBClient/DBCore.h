#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "../../Misc/Misc.h"
#include "../ChatCore/User.h"

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

typedef unsigned int uint;
typedef unsigned long long ullong;

const extern char config_file[];
class DBCore
{
protected:
    const std::string server;
    const std::string port;
    const std::string dbuser;
    const std::string dbpass;
    const std::string dbname;
    const std::string db_character_set;

public:
    int provider;
    DBCore();
    virtual ~DBCore() = default;
    virtual void initialize() = 0;

    virtual std::shared_ptr<User> getUserByID(ullong userID) = 0;
    virtual std::shared_ptr<User> getUserByLogin(std::string userLogin) = 0;

    virtual void hello() = 0;
};