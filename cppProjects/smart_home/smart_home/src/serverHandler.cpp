#include "serverHandler.hpp"

#include "create_tcp_server.hpp"
#include "server_msgs_recveiver.hpp"
#include "server_new_client_receiver.hpp"


namespace advcpp
{

ServerHandler::ServerHandler(WaitableQueue<SensorMsg>& a_msgQueue, AgentsMng& a_agentsMng) 
: m_msgFunc(new ServerMsgsReceiver(a_msgQueue))
, m_newClientFunc(new ServerNewClientsReceiver(a_agentsMng))
, m_server(CreateTcpServer(1200, m_msgFunc, m_newClientFunc))
{    
}

ServerHandler::~ServerHandler() 
{
    
}

void ServerHandler::Run() 
{
    m_server->Run();    
}

void ServerHandler::Send(const char* a_buffer, size_t a_size, int a_client) 
{
    m_server->Send(a_buffer, a_size, a_client);   
}
   
} // namespace advcpp
