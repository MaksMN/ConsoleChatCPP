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

std::shared_ptr<User> ODBC::getUserByID(const ullong &userID, uint &db_error_number)
{
    std::string query = "SELECT * FROM `users` INNER JOIN hash_tab ON `users`.id = `hash_tab`.uid WHERE `id` = '" + std::to_string(userID) + "' LIMIT 1;";
    if (dbQuery(query, db_error_number) != SQL_SUCCESS)
        return nullptr;

    ullong id;
    char login[20];
    char email[100];
    wchar_t first_name[100];
    char last_name[100];
    ullong registered;
    int status;
    ullong session_key;
    char hash[40];
    char salt[40];

    SQLLEN sql_str_length;

    SQLBindCol(sqlStmtHandle, 1, SQL_C_UBIGINT, &id, sizeof(id), nullptr);
    SQLBindCol(sqlStmtHandle, 2, SQL_CHAR, &login, 20, &sql_str_length);
    SQLBindCol(sqlStmtHandle, 3, SQL_CHAR, &email, 100, nullptr);
    SQLBindCol(sqlStmtHandle, 4, SQL_WCHAR, &first_name, 100, &sql_str_length);
    SQLBindCol(sqlStmtHandle, 5, SQL_CHAR, &last_name, 100, nullptr);
    SQLBindCol(sqlStmtHandle, 6, SQL_C_UBIGINT, &registered, sizeof(registered), nullptr);
    SQLBindCol(sqlStmtHandle, 7, SQL_INTEGER, &status, sizeof(status), nullptr);
    SQLBindCol(sqlStmtHandle, 8, SQL_C_UBIGINT, &session_key, sizeof(session_key), nullptr);
    SQLBindCol(sqlStmtHandle, 10, SQL_VARCHAR, &hash, sizeof(hash), nullptr);
    SQLBindCol(sqlStmtHandle, 11, SQL_VARCHAR, &salt, sizeof(salt), nullptr);
    SQLFetch(sqlStmtHandle);

    std::wstring s(first_name, (int)sql_str_length / 2);
    std::wcout << s;
    return nullptr;
}

std::shared_ptr<User> ODBC::getUserByLogin(const std::string &userLogin, uint &db_error_number)
{
    return std::shared_ptr<User>();
}

ullong ODBC::getCount(std::string table, std::string where, uint &db_error_number)
{
    db_error_number = 0;
    ullong count = 0;
    std::string query = "SELECT COUNT(*) FROM `" + table + "` WHERE " + where + ";";
    if (dbQuery(query, db_error_number) != SQL_SUCCESS)
    {
        return 0;
    }
    // Объявление структуры данных для результата запроса версии SQL
    SQLLEN rowsCount;
    SQLUBIGINT V_OD_err, V_OD_id;

    V_OD_err = SQLBindCol(sqlStmtHandle, 1, SQL_C_UBIGINT, &V_OD_id, sizeof(V_OD_id), nullptr);
    SQLFetch(sqlStmtHandle);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return V_OD_id;
}

std::string ODBC::userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number)
{
    return std::string();
}

bool ODBC::saveUser(std::shared_ptr<User> user, uint &db_error_number)
{
    return false;
}

bool ODBC::addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number)
{
    return false;
}

std::shared_ptr<Message> ODBC::getMessageByID(const ullong &messageID, uint &db_error_number)
{
    return std::shared_ptr<Message>();
}

bool ODBC::addMessage(std::shared_ptr<Message> &message, uint &db_error_number)
{
    return false;
}

std::string ODBC::messageList(ullong &reader_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number)
{
    return std::string();
}

std::string ODBC::messageList(ullong &reader_id, ullong interlocutor_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number)
{
    return std::string();
}

int ODBC::dbQuery(std::string &query, uint &db_error_number)
{
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
    {
        wrongDescriptorMsg();
        db_error_number = 1;
        return 1;
    }
    db_error_number = 0;
    std::wstring w_query = Misc::toWstring(query);
    int result = SQLExecDirectW(sqlStmtHandle, w_query.data(), SQL_NTS);
    if (result != SQL_SUCCESS)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        Misc::printMessage("ODBC: Failed to complete the query " + query);
        db_error_number = 1;
    }

    return result;
}
