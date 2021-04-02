#ifndef BBCD81D7_B32F_4C48_9888_CABE53BC9643_H__
#define BBCD81D7_B32F_4C48_9888_CABE53BC9643_H__

#include "functor.hpp"
#include "atomic_value.hpp"
#include "threadpool.hpp"
#include "agentsMng.hpp"
#include "sensor_msg.hpp"

namespace advcpp
{

class NotifyRouter : public ObjThreadFunc
{
public:
    NotifyRouter(const SensorMsg& a_msg, AgentsMng& a_agentsMng);
    ~NotifyRouter();

    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    SensorMsg m_msg;
    AgentsMng& m_agentsMng;
};

class ThreadPoolSubmitter : public ObjThreadFunc
{
public:
    ThreadPoolSubmitter(ThreadPool& a_tp, WaitableQueue<SensorMsg>& m_msgQueue, AgentsMng& m_agentsMng);
    ~ThreadPoolSubmitter();
    virtual void operator()();
    virtual ObjThreadFunc* Clone() const;

private:
    ThreadPool& m_tp;
    WaitableQueue<SensorMsg>& m_msgQueue;
    AgentsMng& m_agentsMng;
};

} // namespace advcpp

#endif // BBCD81D7_B32F_4C48_9888_CABE53BC9643_H__
