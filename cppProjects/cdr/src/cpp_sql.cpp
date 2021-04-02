#include "cpp_sql.hpp"
#include "logger.hpp"

#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>  
#include <stdlib.h>
#include <iostream>
namespace advcpp
{

CdrsDataBase::CdrsDataBase(const std::string& a_hostName, const std::string& a_userName, const std::string& a_password)
: m_driver()
, m_con()
{   
    try{
        initConnection(a_hostName, a_userName, a_password);
    }
    catch(const sql::SQLException &a_ex){
        StartLogger().Exception(a_ex);
        throw;
    }
}

CdrsDataBase::~CdrsDataBase()
{
    m_con->close();
    delete m_con;
}

void CdrsDataBase::QueryExecute(const std::string& a_query) 
{
    try{
        sql::Statement *stmt;
        stmt = m_con->createStatement();
        sql::ResultSet* res;
        res = stmt->executeQuery(a_query.c_str()); 
        delete res;
        delete stmt;    
    }
    catch (sql::SQLException & a_ex) {
        StartLogger().Exception(a_ex);
        StartLogger().Message(a_query.c_str());
  }    
}

bool CdrsDataBase::IsExist(const std::string& a_query) 
{
    bool result = false;
    try{

        sql::Statement *stmt;
        stmt = m_con->createStatement();
        sql::ResultSet* res;
        res = stmt->executeQuery(a_query.c_str()); 
        result = res->rowsCount() > 0;
        delete res;
        delete stmt;  
        return result;
    }
    catch (sql::SQLException & a_ex) {
        StartLogger().Exception(a_ex);
        StartLogger().Message(a_query.c_str());
  }   
  return result;     
}


void CdrsDataBase::initConnection(const std::string& a_hostName, const std::string& a_userName, const std::string& a_password) 
{
    try{
        m_driver = get_driver_instance();
        m_con = m_driver->connect(a_hostName.c_str(), a_userName.c_str(), a_password.c_str());

    }catch (const sql::SQLException& a_ex ){
        StartLogger().Exception(a_ex);
        throw;    
    }

}

} // advcpp


