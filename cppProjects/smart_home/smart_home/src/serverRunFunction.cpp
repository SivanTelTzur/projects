#include "serverRunFunction.hpp"
#include "sharedptr.hpp"
#include "inet_server.hpp"
#include "logger.hpp"
#include "serverHandler.hpp"

namespace advcpp
{

ServerRecvFunc::ServerRecvFunc(ServerHandler& a_server)
: m_server(a_server)
{
}

ServerRecvFunc::~ServerRecvFunc()
{
}

void ServerRecvFunc::operator()()
{
    try{
        m_server.Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

ObjThreadFunc* ServerRecvFunc::Clone() const
{
    try{
        ServerRecvFunc* copy = new ServerRecvFunc(m_server);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

    
} // namespace advcpp
