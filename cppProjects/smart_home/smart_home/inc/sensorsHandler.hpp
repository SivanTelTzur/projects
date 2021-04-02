#ifndef A60E5B71_387C_483E_B621_AFC11C489FE3_H__
#define A60E5B71_387C_483E_B621_AFC11C489FE3_H__

#include "sensor_msg.hpp"
#include "threadpool.hpp" //atomic value , waiting queue, ObjThreadFunc
#include "event.hpp"
#include "agentsMng.hpp"

//#include "atomic_value.hpp"


namespace advcpp
{
class EventMsgReceiveThreadHandler : private NonCopyable{
public:
    EventMsgReceiveThreadHandler(WaitableQueue<SensorMsg>& a_msgQueue, AgentsMng& a_agentsMng);
    ~EventMsgReceiveThreadHandler();

    void Stop();

private:
    WaitableQueue<SensorMsg>& m_msgQueue;
    AgentsMng& m_agentsMng;
    ThreadPool m_tpNotifyEvents;
    SharedPtr<ObjThreadFunc> m_msgSubmitter;
    Thread m_activateTread;
    bool m_stopped;
};

} // namespace advcpp

#endif // A60E5B71_387C_483E_B621_AFC11C489FE3_H__
