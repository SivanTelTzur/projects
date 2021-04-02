#include "logger.hpp"
#include "exceptions_calsses.hpp"

#include <assert.h>

namespace advcpp
{

Logger::Logger(std::ostream& os)
: m_outStream(os)
{}

void Logger::Exception(std::exception const& a_ex)
{
    write(a_ex.what());
}

void Logger::Message(const std::string& a_message)
{
    write(a_message);
}

void Logger::write(const std::string& a_str)
{
    m_mtxLogger.Lock();
    m_outStream << "[TID: " << pthread_self() << "]";
    m_outStream << a_str << std::endl;
    m_mtxLogger.Unlock();

}

Logger& StartLogger()
{
    static Mutex mtx;
    try{
        mtx.Lock();
    }
    catch (const MutexAlreadyLocked& a_ex){
        assert(!a_ex.what());
    }
    static Logger* log = new Logger();
    try{
        mtx.Unlock();
    }
    catch (const MtxDontBelong & a_mtx){
        assert(!a_mtx.what());
    }
    return *log;
}

} //advcpp
