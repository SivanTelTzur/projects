#include "func_server.hpp"

#include "inet_server.hpp"
#include "create_tcp_server.hpp"
#include "cdr_receiver.hpp"
#include "logger.hpp"
#include "create_query_server.hpp"

namespace advcpp
{

ServerTcpFunc::ServerTcpFunc(size_t a_port, MtWaitingQueue<std::string>& a_msgQueue)
: m_msgQueue(a_msgQueue)
, m_port(a_port)
{
}   

ServerTcpFunc::~ServerTcpFunc()
{
}

void ServerTcpFunc::operator()()
{
    try{
        INetServer& tcpServer = CreateTcpServer(m_port, m_msgQueue);
        tcpServer.Run();
        delete &tcpServer;

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("Thread server \n");
        std::terminate();
    }
}

ObjThreadFunc* ServerTcpFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new ServerTcpFunc(m_port, m_msgQueue);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}
/**************************************************/

ServerRcvFunc::ServerRcvFunc(INetServer& a_server)
: m_server(a_server)
{
}   

ServerRcvFunc::~ServerRcvFunc()
{
}

void ServerRcvFunc::operator()()
{
    try{
        m_server.Run();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("Thread server \n");
        std::terminate();
    }
}

ObjThreadFunc* ServerRcvFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new ServerRcvFunc(m_server);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

/**************************************************/

ReceiverFunc::ReceiverFunc(size_t a_port, MtWaitingQueue<Cdr>& a_cdrsQueue)
: m_cdrsQueue(a_cdrsQueue)
, m_port(a_port)
{
}   

ReceiverFunc::~ReceiverFunc()
{
}


void ReceiverFunc::operator()()
{
    try{
        CdrReceiver cdrReceiver(m_port, m_cdrsQueue);
        cdrReceiver.Start();

    }catch(const std::exception& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("Thread server \n");
        std::terminate();
    }
}

ObjThreadFunc* ReceiverFunc::Clone() const
{
    try{
        ObjThreadFunc* copy = new ReceiverFunc(m_port, m_cdrsQueue);
        return copy;
    }
    catch (std::bad_alloc & a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }
}

} // namespace advcpp
