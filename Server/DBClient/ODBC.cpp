#include "ODBC.h"

void ODBC::initialize()
{
    std::wstring w_server = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(server.data());
    std::wstring w_port = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(port.data());
    std::wstring w_dbuser = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(dbuser.data());
    std::wstring w_dbpass = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(dbpass.data());
    std::wstring w_dbname = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(dbname.data());

    connect_data = L"DRIVER={MySQL ODBC 8.0 ANSI Driver};SERVER=" + w_server +
                   L";PORT=" + w_port +
                   L";DATABASE=" + w_dbname +
                   L";UID=" + w_dbuser +
                   L";PWD=" + w_dbpass + L";";

    constexpr auto SQL_RESULT_LEN = 240;
    constexpr auto SQL_RETURN_CODE_LEN = 1024;
    SQLWCHAR retconstring[SQL_RETURN_CODE_LEN]{}; // строка для кода возврата из функций API ODBC

    // Выделяем дескриптор для базы данных
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
    {
        wrongDescriptorMsg();
        return;
    }

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
    {
        wrongDescriptorMsg();
        return;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
    {
        wrongDescriptorMsg();
        return;
    }

    // Устанавливаем соединение с сервером
    switch (SQLDriverConnectW(sqlConnHandle,
                              GetDesktopWindow(),
                              (SQLWCHAR *)connect_data.data(),
                              SQL_NTS,
                              retconstring,
                              1024,
                              NULL,
                              SQL_DRIVER_COMPLETE))
    {

    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        Misc::printMessage("Successfully connected to SQL Server");
        break;

    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
        Misc::printMessage("Could not connect to SQL Server");
        complete();
        return;

    default:
        break;
    }
}

void ODBC::hello()
{
    Misc::printMessage("Hello! Im ODBC.");
}

std::shared_ptr<User> ODBC::getUserByID(ullong userID)
{
    return std::shared_ptr<User>();
}

std::shared_ptr<User> ODBC::getUserByLogin(std::string userLogin)
{
    return std::shared_ptr<User>();
}
