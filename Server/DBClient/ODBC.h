#pragma once
#include "SQL.h"

class ODBC : public SQL
{
private:
    /* data */
public:
    ~ODBC() = default;
    void initialize();
    void hello();
};
