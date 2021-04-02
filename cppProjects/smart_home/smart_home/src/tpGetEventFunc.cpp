#include "tpGetEventFunc.hpp"

namespace advcpp
{

NotifyRouter::NotifyRouter(const SensorMsg& a_msg, AgentsMng& a_agentsMng) 
: m_msg(a_msg)
, m_agentsMng(a_agentsMng)
{   

}

NotifyRouter::~NotifyRouter() 
{   
}

void NotifyRouter::operator()() 
{
    SharedPtr<IAgent> found;
    m_agentsMng.FindAgent(m_msg.m_ip, found);
    found->testMe(); //DELETE!!
    found->Notify(m_msg.m_msg);
}

ObjThreadFunc* NotifyRouter::Clone() const
{
    ObjThreadFunc* copy = new NotifyRouter(m_msg, m_agentsMng);
    return copy;    
}

ThreadPoolSubmitter::ThreadPoolSubmitter(ThreadPool& a_tp, WaitableQueue<SensorMsg>& a_msgQueue, AgentsMng& a_agentsMng) 
: m_tp(a_tp)
, m_msgQueue(a_msgQueue)
, m_agentsMng(a_agentsMng)
{   
}

ThreadPoolSubmitter::~ThreadPoolSubmitter() 
{
    
}

void ThreadPoolSubmitter::operator()() 
{
    while (true){
        SensorMsg msg;
        m_msgQueue.Dequeue(msg);
        SharedPtr<ObjThreadFunc> task(new NotifyRouter(msg, m_agentsMng));
        try{
            m_tp.SubmitTask(task);
        }
        catch (const ThreadPoolClosedForNewTasks& a_ex){
            break;
        }
    }
}

ObjThreadFunc* ThreadPoolSubmitter::Clone() const
{
    ObjThreadFunc* copy = new ThreadPoolSubmitter(m_tp, m_msgQueue, m_agentsMng);
    return copy;
}

    
} // namespace advcpp
