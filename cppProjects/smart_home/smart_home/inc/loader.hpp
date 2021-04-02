#ifndef C1949D61_9086_4908_BA75_B968CDDB24E7_H__
#define C1949D61_9086_4908_BA75_B968CDDB24E7_H__

#include "iagent.hpp"
#include "serverHandler.hpp"
#include "eventsRouter.hpp"


#include <fstream>

namespace advcpp
{

class Loader : private NonCopyable
{
public:
    typedef IAgent* (*CreateAgent)(EventsRouter& a_router, ServerHandler& a_server
                    , SubscribersMng& a_subscription);
    enum { NUM_OF_CONFIG_VALUES = 6 };

    Loader(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription, AgentsMng& a_agentsMng);
    ~Loader();

    void OpenConfigFile(const std::string& a_fileName);

private:
    void parseConfigFile();
    void updateDeviceMember(const std::string& a_line, std::string& a_member, size_t a_memPlaceNum);
    int createAgentFromSo(Config& a_deviceConfig);
private:
    EventsRouter& m_router;
    ServerHandler& m_server;
    SubscribersMng& m_subscription;
    AgentsMng& m_agentsMng;
    std::ifstream m_fp;
};

} // namespace advcpp

#endif // C1949D61_9086_4908_BA75_B968CDDB24E7_H__
