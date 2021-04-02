#ifndef C1877F63_5FB2_483D_9F59_EDFB0CF391D4_H__
#define C1877F63_5FB2_483D_9F59_EDFB0CF391D4_H__

#include "eventsRouter.hpp"
#include "serverHandler.hpp"
#include "config_struct.hpp"
#include "sensorsHandler.hpp"


namespace advcpp
{

class CentralHub : NonCopyable
{
public:
    CentralHub();
    ~CentralHub();

    void Run();

private:
    void stop();
    void loadAgents();

private:
    WaitableQueue<SensorMsg> m_msgs;
    AgentsMng m_agents;
    SubscribersMng* m_subscribers;
    EventsRouter* m_router;
    ServerHandler* m_server;
    //EventMsgReceiveThreadHandler m_eventsRecvThread;
};


  
} // namespace advcpp

#endif // C1877F63_5FB2_483D_9F59_EDFB0CF391D4_H__
