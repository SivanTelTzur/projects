#include "TemperatureAgent.hpp"

#include <arpa/inet.h> // htons
#include <cstring>
namespace advcpp
{

AtomicValue<int> TemperatureAgent::m_counter(0);

TemperatureAgent::TemperatureAgent(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription) 
: m_server(a_server)
, m_router(a_router)
, m_subscription(a_subscription)
, m_deviceData()
{
    
}

TemperatureAgent::~TemperatureAgent() 
{
    
}

void TemperatureAgent::InitAgent(const Config& a_config, SharedPtr<IAgent> a_pThis) 
{
    m_deviceData = a_config;    
}

void TemperatureAgent::Subscribe(Topic a_topic, SharedPtr<IAgent> a_pThis) 
{
    m_subscription.Subscribe(a_topic, a_pThis);
}

void TemperatureAgent::testMe() const
{
    ++m_counter;
    std::cerr << "I'm in testMe" << std::endl;      
}

void TemperatureAgent::Notify(const std::string& a_msg) 
{
    Event event;
    fillEvent(a_msg, event);
    m_router.Notify(event);
}

void TemperatureAgent::Update(const Event a_event) const 
{
    unsigned int bufferSize;
    char* msg = PackEvent(&bufferSize, a_event); 
    m_server.Send(msg, bufferSize, m_deviceData.m_socket);
    delete msg;    
}

void TemperatureAgent::UpdateSocket(int a_socketUpdate) 
{
    m_deviceData.m_socket = a_socketUpdate;
}

void TemperatureAgent::fillEvent(const std::string& a_msg, Event& a_toFill) const 
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

char* TemperatureAgent::PackEvent(unsigned int* a_buffersize, const Event a_event) const 
{
    *a_buffersize = a_event.m_timestamp.size() 
                    + a_event.m_topic.m_eventType.size() + a_event.m_payLoad.size() +
                    + a_event.m_topic.m_location.m_room.size() + a_event.m_topic.m_location.m_floor.size()
                    + 5 + sizeof(unsigned int); //5 stand for the size of each element 

    char* initialBuffer = new char[*a_buffersize];
    char* buffer = initialBuffer;
    *((unsigned int*)buffer) = htonl(*a_buffersize - sizeof(unsigned int));
    buffer = buffer + sizeof(unsigned int); 

    *buffer++ = a_event.m_timestamp.size(); 
    strncpy(buffer, a_event.m_timestamp.c_str(), a_event.m_timestamp.size());
    buffer = buffer + a_event.m_timestamp.size();

    *buffer++ = a_event.m_topic.m_eventType.size(); 
    strncpy(buffer, a_event.m_topic.m_eventType.c_str(), a_event.m_topic.m_eventType.size());
    buffer = buffer + a_event.m_topic.m_eventType.size();

    *buffer++ = a_event.m_payLoad.size(); 
    strncpy(buffer, a_event.m_payLoad.c_str(), a_event.m_payLoad.size());
    buffer = buffer + a_event.m_payLoad.size();

    *buffer++ = a_event.m_topic.m_location.m_room.size(); 
    strncpy(buffer, a_event.m_topic.m_location.m_room.c_str(), a_event.m_topic.m_location.m_room.size());
    buffer = buffer + a_event.m_topic.m_location.m_room.size();

    *buffer++ = a_event.m_topic.m_location.m_floor.size(); 
    strncpy(buffer, a_event.m_topic.m_location.m_floor.c_str(), a_event.m_topic.m_location.m_floor.size());
    buffer = buffer + a_event.m_topic.m_location.m_floor.size();

    buffer = buffer - *a_buffersize;
    return buffer;
}

IAgent* CreateMeAnAgent(EventsRouter& a_router, ServerHandler& a_server, SubscribersMng& a_subscription) 
{
    return new TemperatureAgent(a_router, a_server, a_subscription);    
}

} // namespace advcpp
