#ifndef EABA3958_B4EE_44BD_95D5_CDA471D55B16
#define EABA3958_B4EE_44BD_95D5_CDA471D55B16

#include "inew_client_action.hpp"
//#include "agentsMng.hpp"

namespace advcpp
{
class AgentsMng;
class ServerNewClientsReceiver : public INewClientHandlerFunc
{
public:
    explicit ServerNewClientsReceiver(AgentsMng& a_agentsMng);
    ~ServerNewClientsReceiver ();
    virtual void NewClient(int a_clientSocket, const std::string& a_ip);

private:
    AgentsMng& m_agentsMng;
};
    
} // namespace advcpp

#endif // EABA3958_B4EE_44BD_95D5_CDA471D55B16
