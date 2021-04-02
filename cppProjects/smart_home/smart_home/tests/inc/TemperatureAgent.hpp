#ifndef BBC71D22_310B_4FC0_B1F2_A3531BBF9661_H__
#define BBC71D22_310B_4FC0_B1F2_A3531BBF9661_H__

#include "iagent.hpp"
#include "eventsRouter.hpp"
#include "subscribersMng.hpp"
#include "serverHandler.hpp"

#include <iostream>
namespace advcpp
{
    
class TemperatureAgent : public IAgent
{
public:
    TemperatureAgent(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription);
    ~TemperatureAgent();  

    virtual void InitAgent(const Config& a_config, SharedPtr<IAgent> a_pThis);
    virtual void Subscribe(Topic a_topic, SharedPtr<IAgent> a_pThis);

    virtual void Notify(const std::string& a_msg);

    //tests func
    virtual void testMe()const;

    virtual void Update(const Event a_event) const ;
    virtual void UpdateSocket(int a_socketUpdate);

    static int GetCounter() {return m_counter.Get();}
    static void ResetCounter() {m_counter.Reset();}

    virtual const Config& GetDeviceData() const {return m_deviceData;}

private:
    void fillEvent(const std::string& a_msg, Event& a_toFill) const;
    char* PackEvent(unsigned int* a_buffersize, const Event a_event) const;

private: 
    static AtomicValue<int> m_counter;
    ServerHandler& m_server;
    EventsRouter& m_router;
    SubscribersMng& m_subscription; 
    Config m_deviceData;
};

IAgent* CreateMeAnAgent(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription) ;

} // namespace advcpp

#endif // BBC71D22_310B_4FC0_B1F2_A3531BBF9661_H__
