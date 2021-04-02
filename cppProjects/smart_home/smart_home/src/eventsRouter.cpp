#include "eventsRouter.hpp"

#include <iostream>

namespace advcpp
{
EventsRouter::EventsRouter(SubscribersMng& a_subscribers) 
: m_subscribers(a_subscribers)
, m_events()
{
    
}

EventsRouter::~EventsRouter() 
{
    
}

void EventsRouter::Run() 
{
    while(true){
        Event event;
        m_events.Dequeue(event);
        updateSubscribers(event);
    }    
}

void EventsRouter::Notify(Event a_newEvent) 
{
    m_events.Enqueue(a_newEvent);
    std::cerr << "EventsRouter::Notify  " << a_newEvent.m_topic.m_eventType <<std::endl;    
}

void EventsRouter::updateSubscribers(Event a_newEvent) 
{
    m_subscribers.UpdateSubscribers(a_newEvent);
    std::cerr << "Update subscribers  " << a_newEvent.m_topic.m_eventType <<std::endl;
}


} // namespace advcpp
