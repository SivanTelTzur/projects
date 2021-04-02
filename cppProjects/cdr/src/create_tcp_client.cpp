#include "create_tcp_client.hpp"
#include "tcp_client.hpp"
#include "logger.hpp"

#include <exception>

namespace advcpp
{

INetClient& CreateTcpClient(const std::string& a_ip, size_t a_port)
{
    try{
        INetClient* create = new TcpClient(a_ip, a_port);
        return *create;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}
} // namespace advcpp