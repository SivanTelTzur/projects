#include "create_query_server.hpp"
#include "queries_tcp_server.hpp"
#include "logger.hpp"

#include <exception>

namespace advcpp
{
INetServer& CreateQueriesServer(size_t a_port, MtWaitingQueue<Query>& a_msgQueue) 
{
    try{
        INetServer* create = new QueriesTcpServer(a_port, a_msgQueue);
        return *create;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }    
}

} // namespace advcpp
