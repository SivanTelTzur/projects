#include "create_tcp_server.hpp"
#include "tcp_server.hpp"
#include "logger.hpp"

#include <exception>

namespace advcpp
{
INetServer& CreateTcpServer(size_t a_port, MtWaitingQueue<std::string>& a_msgQueue) 
{
    try{
        INetServer* create = new TcpServer(a_port, a_msgQueue);
        return *create;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }    
}

} // namespace advcpp
