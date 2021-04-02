#include "sensorsHandler.hpp"
#include "tpGetEventFunc.hpp"


namespace advcpp
{

EventMsgReceiveThreadHandler::EventMsgReceiveThreadHandler(WaitableQueue<SensorMsg>& a_msgQueue, AgentsMng& a_agentsMng) 
: m_msgQueue(a_msgQueue)
, m_agentsMng(a_agentsMng)
, m_tpNotifyEvents(30)
, m_msgSubmitter(new ThreadPoolSubmitter(m_tpNotifyEvents, m_msgQueue, m_agentsMng))
, m_activateTread(m_msgSubmitter)
, m_stopped(false)   
{  
 
}

EventMsgReceiveThreadHandler::~EventMsgReceiveThreadHandler() 
{
    if (!m_stopped){
        //m_msgQueue.Finish(0);
        m_tpNotifyEvents.ShutdownImmediately();
        m_activateTread.Join();
    }
}  

void EventMsgReceiveThreadHandler::Stop() 
{
    if (!m_stopped){
        m_msgQueue.Finish(0);
        m_tpNotifyEvents.Shutdown();
        m_activateTread.Join(); 
    }
    m_stopped = true;
}
    
} // namespace advcpp
