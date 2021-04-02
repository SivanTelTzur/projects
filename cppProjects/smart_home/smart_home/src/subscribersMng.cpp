#include "subscribersMng.hpp"
#include "mutex_guard.hpp"

#include <algorithm> //find_if
#include <iostream>

namespace advcpp
{
SubscribersMng::SubscribersMng() 
: m_subscribers()
, m_mapMtx()
{
    
}

SubscribersMng::~SubscribersMng() 
{
    
}

void SubscribersMng::Subscribe(const Topic a_topic, SharedPtr<IAgent> a_Agent) 
{
    addSubscriber(a_topic, a_Agent);
}

void SubscribersMng::Unsubscribe(const Topic a_topic, SharedPtr<IAgent> a_Agent) 
{ 
    removeSubscriber(a_topic, a_Agent);
}

void SubscribersMng::addSubscriber(const Topic a_topic, SharedPtr<IAgent> a_Agent) 
{
    MutexGuard mtxGuard(m_mapMtx);
    m_subscribers.insert(std::make_pair(a_topic, a_Agent));
    std::cerr << "I'm here:  subscriber insert" << m_subscribers.size() << std::endl;
}

void SubscribersMng::removeSubscriber(const Topic a_topic, SharedPtr<IAgent> a_Agent) 
{
    MutexGuard mtxGuard(m_mapMtx);
    std::pair<It,It> range = m_subscribers.equal_range(a_topic);
    It current = range.first;
    while (current != range.second){
        if (a_Agent == (*current).second){
            m_subscribers.erase(current);
            return;
        }
        ++current;
    }
    mtxGuard.UnlockMtx();
    assert(!"Topic wasn't found removeSubscriber failed");   
}

void SubscribersMng::UpdateSubscribers(Event a_newEvent) const
{
    updateSubscribers(a_newEvent);    
}

void SubscribersMng::updateSubscribers(Event a_newEvent) const
{
    MutexGuard mtxGuard(m_mapMtx);
    std::pair<CIt,CIt> range = m_subscribers.equal_range(a_newEvent.m_topic);
    CIt current = range.first;
    while (current != range.second){
        current->second->Update(a_newEvent);
        ++current;
    }        
}

    


} // namespace advcpp
