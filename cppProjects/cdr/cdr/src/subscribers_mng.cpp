#include "subscribers_mng.hpp"
#include "logger.hpp"

#include <exception>

namespace advcpp
{

SubscribersMng::SubscribersMng(MtWaitingQueue<Subscriber>& a_subscribersQueue, SubscriberData& a_subData) 
: m_subscribers(a_subscribersQueue)
, m_subData(a_subData)
{    
}

SubscribersMng::~SubscribersMng() 
{  
}

void SubscribersMng::Start() 
{        
    start();
}

void SubscribersMng::start() 
{
    for (;;){
        try{
            Subscriber newSub = m_subscribers.Dequeue();
            m_subData.InsertData(newSub); 
        }
        catch (const std::exception& a_ex){
            StartLogger().Exception(a_ex);
            StartLogger().Message("insert subscriber");
            std::terminate();
        }
    }
    
}
      
} // namespace advcpp
