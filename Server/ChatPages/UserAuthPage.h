#include "IPagesCore.h"

class UserAuthPage final : public IPagesCore
{
private:
    /* data */
public:
    UserAuthPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient);
    ~UserAuthPage() = default;

    void run() override;
    bool commandHandler() override;

private:
    void offerRegisterOrLogin();
};
