#include "server_new_client_receiver.hpp"
#include "agentsMng.hpp"
namespace advcpp
{

ServerNewClientsReceiver::ServerNewClientsReceiver(AgentsMng& a_agentsMng) 
: m_agentsMng(a_agentsMng)
{
}

ServerNewClientsReceiver::~ServerNewClientsReceiver() 
{    
}

void ServerNewClientsReceiver::NewClient(int a_clientSocket, const std::string& a_ip) 
{
    SharedPtr<IAgent> found;
    m_agentsMng.FindAgent(a_ip, found);
    found->UpdateSocket(a_clientSocket);       
} 
   
} // namespace advcpp
