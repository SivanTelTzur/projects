#include "agent.hpp"

namespace advcpp
{

AtomicValue<int> Agent::m_counter(0);

Agent::Agent(EventsRouter& a_router, ServerHandler& a_server) : AbcAgent(a_router, a_server)
{
}

void Agent::Subscribe(Topic a_topic, SharedPtr<AbcAgent> a_pThis) 
{
    
}

void Agent::Notify(const std::string& a_msg) 
{
    ++m_counter;
}

void Agent::Publish(const std::string& a_msg) const
{
    Event event;
    fillEvent(a_msg, event);
    AbcAgent::NotifyRouter(event);    
}

void Agent::Update(const std::string &a_msg) const 
{
    
}

void Agent::fillEvent(const std::string& a_msg, Event& a_toFill) const
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
    //std::cout << "time : " << timestamp << std::endl;    
}  

} // namespace advcpp
