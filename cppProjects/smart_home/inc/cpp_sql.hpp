#ifndef FD759826_3EEA_48AE_9F7F_D8597A095135_H__
#define FD759826_3EEA_48AE_9F7F_D8597A095135_H__

#include "mysql_connection.h"
#include "noncopyable.hpp"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <string>


namespace advcpp
{
class CdrsDataBase : private NonCopyable
{
public:
    CdrsDataBase(const std::string& a_hostName, const std::string& a_userName, const std::string& a_password);
    ~CdrsDataBase();

    void QueryExecute(const std::string& a_query);
    bool IsExist(const std::string& a_query);

private:
    void initConnection(const std::string& a_hostName, const std::string& a_userName, const std::string& a_password);

private:
    sql::Driver *m_driver;
    sql::Connection *m_con;

};

class CdrsDBDriverFailedException : public std::exception{
public:
    virtual const char* what() const throw();
};


class CdrsDBConnectionFailedException : public std::exception{
public:
    virtual const char* what() const throw();
};

//class CdrsDBStatementFailedException : public std::exception{
//public:
//    virtual const char* what() const throw();
//};

} // namespace advcpp


#endif // FD759826_3EEA_48AE_9F7F_D8597A095135_H__