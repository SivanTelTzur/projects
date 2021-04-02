#ifndef FD941873_9590_4CCC_9C08_8D2277BBA194_H__
#define FD941873_9590_4CCC_9C08_8D2277BBA194_H__

#include "iserver.hpp"
#include "inet_server.hpp"
#include "Imsg_handler_func.hpp"
#include "waitableQueue.hpp"
#include "sensor_msg.hpp"
#include "inew_client_action.hpp"
#include "sharedptr.hpp"

namespace advcpp
{

class AgentsMng;
class ServerHandler : public IServer
{
public:
    ServerHandler(WaitableQueue<SensorMsg>& a_msgQueue, AgentsMng& a_agentsMng);
    ~ServerHandler();

    void Run();
    virtual void Send(const char* a_buffer, size_t a_size, int a_client);

private:
    SharedPtr<IMsgHandlerFunc> m_msgFunc;
    SharedPtr<INewClientHandlerFunc> m_newClientFunc;
    SharedPtr<INetServer> m_server;
   
};
    
} // namespace advcpp


#endif // FD941873_9590_4CCC_9C08_8D2277BBA194_H__