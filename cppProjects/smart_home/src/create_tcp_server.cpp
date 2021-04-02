#include "create_tcp_server.hpp"
#include "new_tcp_server.hpp"
#include "logger.hpp"

#include <exception>

namespace advcpp
{
INetServer* CreateTcpServer(size_t a_port, SharedPtr<IMsgHandlerFunc> a_getMsgFunction, SharedPtr<INewClientHandlerFunc> a_newClientFunc) 
{
    try{
        INetServer* create = new TcpServer(a_port, a_getMsgFunction, a_newClientFunc);
        return create;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }    
}

} // namespace advcpp
