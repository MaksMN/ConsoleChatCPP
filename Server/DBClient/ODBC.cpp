#include "ODBC.h"

void ODBC::initialize()
{
    connect_data = "DRIVER={MySQL ODBC 8.0 Unicode Driver};charset=utf8mb4;SERVER=" + server +
                   ";PORT=" + port +
                   ";DATABASE=" + dbname +
                   ";UID=" + dbuser +
                   ";PWD=" + dbpass + ";";

    constexpr auto SQL_RESULT_LEN = 240;
    constexpr auto SQL_RETURN_CODE_LEN = 1024;
    SQLCHAR retconstring[SQL_RETURN_CODE_LEN]{}; // строка для кода возврата из функций API ODBC

    // Выделяем дескриптор для базы данных
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
    {
        diagInfo(SQL_HANDLE_ENV, sqlEnvHandle, "SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle)", "no query");
        wrongDescriptorMsg();
        return;
    }

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
    {
        diagInfo(SQL_HANDLE_ENV, sqlEnvHandle, "SQLSetEnvAttr", "no query");
        wrongDescriptorMsg();
        return;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
    {
        diagInfo(SQL_HANDLE_ENV, sqlEnvHandle, "SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle)", "no query");
        wrongDescriptorMsg();
        return;
    }

    // Устанавливаем соединение с сервером
    switch (SQLDriverConnectA(sqlConnHandle,
                              GetDesktopWindow(),
                              (SQLCHAR *)connect_data.data(),
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
        diagInfo(SQL_HANDLE_DBC, sqlConnHandle, "SQLDriverConnectA", "no query");
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
    db_error_number = 0;
    std::string query = "SELECT * FROM `users` INNER JOIN hash_tab ON `users`.id = `hash_tab`.uid WHERE `id` = '" + std::to_string(userID) + "' LIMIT 1;";
    int res = dbQuery(query, db_error_number);
    if (res <= 0)
        return nullptr;
    auto user = fetchUserRow(db_error_number);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return user;
}

std::shared_ptr<User> ODBC::getUserByLogin(const std::string &userLogin, uint &db_error_number)
{
    db_error_number = 0;
    std::string query = "SELECT * FROM `users` INNER JOIN hash_tab ON `users`.id = `hash_tab`.uid WHERE `id` = '" + userLogin + "' LIMIT 1;";
    int res = dbQuery(query, db_error_number);
    if (res <= 0)
        return nullptr;
    auto user = fetchUserRow(db_error_number);
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    return user;
}

ullong ODBC::getCount(std::string table, std::string where, uint &db_error_number)
{
    db_error_number = 0;
    ullong count = 0;
    std::string query = "SELECT COUNT(*) FROM `" + table + "` WHERE " + where + ";";
    int res = dbQuery(query, db_error_number);
    if (res <= SQL_SUCCESS)
    {
        return 0;
    }

    try
    {
        BindCol(sqlStmtHandle, 1, SQL_INTEGER, &res, sizeof(res), nullptr);
    }
    catch (BindColException &e)
    {
        db_error_number = 1;
        std::cout << '\n'
                  << e.what() << '\n';
        Misc::printMessage("getCount");
        return 0;
    }
    if (Fetch(db_error_number) != SQL_SUCCESS)
        return 0;
    return res;
}

std::string ODBC::userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number)
{
    db_error_number = 0;
    std::string query;

    ullong count = getCount("users", "`id` != 0", db_error_number);

    Misc::alignPaginator(start, per_page, count);

    query = "SELECT * FROM `users` WHERE `id` != 0 LIMIT " + std::to_string(start - 1) + ", " + std::to_string(start + per_page) + ";";

    uint res = dbQuery(query, db_error_number);
    capacity = (ullong)res;

    if (res <= 0)
        return std::string();
    std::string result;
    for (int i{0}; i < res; i++)
    {
        result += std::to_string(i + 1) + ". "; // порядковый номер
        auto user = fetchUserRow(db_error_number, 1, false);
        result += user->userData();
        result += "\n";
    }
    return result;
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

std::shared_ptr<User> ODBC::fetchUserRow(uint &db_error_number, uint startRow, bool getPassData)
{
    ullong id;
    char login[50];
    char email[500];
    char first_name[500];
    char last_name[500];
    ullong registered;
    int status;
    ullong session_key;
    std::string hash;
    std::string salt;
    char db_hash[41];
    char db_salt[41];

    SQLLEN login_length;
    SQLLEN email_length;
    SQLLEN first_name_length;
    SQLLEN last_name_length;
    SQLLEN hash_length;
    SQLLEN salt_length;
    try
    {
        BindCol(sqlStmtHandle, startRow, SQL_C_UBIGINT, &id, sizeof(id), nullptr);
        BindCol(sqlStmtHandle, ++startRow, SQL_CHAR, &login, 20, &login_length);
        BindCol(sqlStmtHandle, ++startRow, SQL_CHAR, &email, 500, &email_length);
        BindCol(sqlStmtHandle, ++startRow, SQL_CHAR, &first_name, 500, &first_name_length);
        BindCol(sqlStmtHandle, ++startRow, SQL_CHAR, &last_name, 500, &last_name_length);
        BindCol(sqlStmtHandle, ++startRow, SQL_C_UBIGINT, &registered, sizeof(registered), nullptr);
        BindCol(sqlStmtHandle, ++startRow, SQL_INTEGER, &status, sizeof(status), nullptr);
        BindCol(sqlStmtHandle, ++startRow, SQL_C_UBIGINT, &session_key, sizeof(session_key), nullptr);

        if (getPassData)
        {
            ++startRow;
            BindCol(sqlStmtHandle, ++startRow, SQL_CHAR, &db_hash, 41, &hash_length);
            BindCol(sqlStmtHandle, ++startRow, SQL_CHAR, &db_salt, 41, &salt_length);
        }
    }
    catch (BindColException &e)
    {
        db_error_number = 1;
        std::cout << '\n'
                  << e.what() << '\n';
        Misc::printMessage("fetchUserRow");
    }

    int res = Fetch(db_error_number);

    if (res == SQL_SUCCESS)
    {

        if (getPassData)
        {
            hash = std::string(db_hash, hash_length);
            salt = std::string(db_salt, salt_length);
        }

        auto user = std::make_shared<User>(
            id,
            std::string(login, login_length),
            std::string(email, email_length),
            std::string(first_name, first_name_length),
            std::string(last_name, last_name_length),
            registered,
            (user::status)status,
            session_key,
            hash,
            salt);
        return user;
    }

    return nullptr;
}

int ODBC::dbQuery(std::string &query, uint &db_error_number)
{
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    db_error_number = 0;

    SQLINTEGER res = SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
    if (res != SQL_SUCCESS)
    {
        diagInfo(SQL_HANDLE_STMT, sqlStmtHandle, "SQLAllocHandle in dbQuery sqlStmtHandle", query);
        diagInfo(SQL_HANDLE_DBC, sqlConnHandle, "SQLAllocHandle in dbQuery sqlConnHandle", query);
        db_error_number = 1;
        return -1;
    }

    int result = SQLExecDirectA(sqlStmtHandle, (SQLCHAR *)query.data(), SQL_NTS);
    if (result != SQL_SUCCESS)
    {
        diagInfo(SQL_HANDLE_STMT, sqlStmtHandle, "SQLExecDirectA", query);
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        Misc::printMessage("ODBC: Failed to complete the query " + query);
        db_error_number = 1;
        return -1;
    }
    SQLLEN RowCountPtr = -1;
    if (result == SQL_SUCCESS)
        SQLRowCount(sqlStmtHandle, &RowCountPtr);

    return RowCountPtr;
}

SQLINTEGER ODBC::Fetch(uint &db_error_number)
{
    db_error_number = 0;
    int res = SQLFetch(sqlStmtHandle);
    if (res != SQL_SUCCESS && res != SQL_NO_DATA)
    {
        diagInfo(SQL_HANDLE_STMT, sqlStmtHandle, "Fetch", "");
        db_error_number = 1;
    }

    return res;
}

void ODBC::BindCol(SQLHSTMT StatementHandle, SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, SQLPOINTER TargetValuePtr, SQLLEN BufferLength, SQLLEN *StrLen_or_IndPtr)
{
    SQLRETURN res = SQLBindCol(StatementHandle, ColumnNumber, TargetType, TargetValuePtr, BufferLength, StrLen_or_IndPtr);
    if (!(res == SQL_SUCCESS || res == SQL_SUCCESS_WITH_INFO))
    {
        diagInfo(SQL_HANDLE_STMT, StatementHandle, "SQLBindCol", "");
        throw BindColException();
    }
}

void ODBC::diagInfo(SQLINTEGER handle_type, SQLHANDLE &handle, const std::string &function, const std::string &query)
{
    SQLCHAR Sqlstate[6];
    for (int i{0}; i < 6; i++)
    {
        Sqlstate[i] = '0';
    }
    SQLINTEGER NativeErrorPtr = 0;
    SQLCHAR MessageText[2048];
    for (int i{0}; i < 2048; i++)
    {
        MessageText[i] = '\0';
    }
    SQLSMALLINT MessageText_len = 0;
    SQLGetDiagRecA(handle_type, handle, 1, Sqlstate, &NativeErrorPtr, MessageText, 2048, &MessageText_len);

    std::string sqlstate;
    std::string msg;

    for (int i{0}; i < 5; i++)
    {
        sqlstate.push_back(Sqlstate[i]);
    }
    for (int i{0}; i < MessageText_len + 1; i++)
    {
        /* SQLGetDiagRec может записывать в сообщение неопределенные байты
         из за чего программа вылетает при преобразовании сообщения в wchar_t
         Оставим только ASCII символы
        */
        if (MessageText[i] >= 0 && MessageText[i] < 127)
            msg.push_back(MessageText[i]);
    }
    Misc::printMessage("\nФункция: " + function);
    Misc::printMessage("Запрос: " + query);
    Misc::printMessage("Sqlstate: " + sqlstate + " Расшифровка https://learn.microsoft.com/ru-ru/sql/odbc/reference/syntax/odbc-api-reference?view=sql-server-ver16");
    Misc::printMessage("NativeError: " + std::to_string((int)NativeErrorPtr));
    Misc::printMessage("Message: " + msg);
    return;
}
