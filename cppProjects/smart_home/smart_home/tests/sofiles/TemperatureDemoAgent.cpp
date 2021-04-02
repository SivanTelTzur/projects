#include "TemperatureDemoAgent.hpp"

namespace advcpp
{



extern "C"
IAgent* CreateMeAnAgent(IRouter& a_router, IServer& a_server, ISubscription& a_subscription) 
{
    return new TemperatureDemoAgent(a_router, a_server, a_subscription);    
}

AtomicValue<int> TemperatureDemoAgent::m_counter(0);

TemperatureDemoAgent::TemperatureDemoAgent(IRouter& a_router, IServer& a_server, ISubscription& a_subscription) 
: m_server(a_server)
, m_router(a_router)
, m_subscription(a_subscription)
, m_deviceData()
{
    
}

TemperatureDemoAgent::~TemperatureDemoAgent() 
{
    
}

void TemperatureDemoAgent::InitAgent(const Config& a_config, SharedPtr<IAgent> a_pThis) 
{
    m_deviceData = a_config;    
}

void TemperatureDemoAgent::Subscribe(Topic a_topic, SharedPtr<IAgent> a_pThis) 
{
    m_subscription.Subscribe(a_topic, a_pThis);    
}

void TemperatureDemoAgent::testMe() const
{
    ++m_counter;
    std::cerr << "I'm in testMe" << std::endl;      
}

void TemperatureDemoAgent::Notify(const std::string& a_msg) 
{
    Event event;
    fillEvent(a_msg, event);
    m_router.Notify(event);
}

void TemperatureDemoAgent::Update(const Event a_event) const
{
    //m_server.Send(a_msg.c_str(), a_msg.size(), m_deviceData.m_socket);    
}

void TemperatureDemoAgent::UpdateSocket(int a_socketUpdate) 
{
    m_deviceData.m_socket = a_socketUpdate;
}

void TemperatureDemoAgent::fillEvent(const std::string& a_msg, Event& a_toFill) const 
{
    Event event;
    const char* buffer = a_msg.c_str();

    size_t currentLen = *buffer++;//(size_t)(*(buffer));
    event.m_timestamp.append(buffer, currentLen);
    buffer = buffer + currentLen;

    currentLen = *buffer++;
    event.m_topic.m_eventType.append(buffer, currentLen);
    buffer = buffer + currentLen;

    currentLen = *buffer++;
    event.m_payLoad.append(buffer, currentLen);
    buffer = buffer + currentLen;

    event.m_topic.m_location.m_room = this->m_deviceData.m_room;
    event.m_topic.m_location.m_floor = this->m_deviceData.m_floor;

    a_toFill = event;
}



} // namespace advcpp
