#ifndef E4AAC306_5CC5_433D_AC74_16234D1D2883_H__
#define E4AAC306_5CC5_433D_AC74_16234D1D2883_H__

#include "abcAgent.hpp"
#include "event.hpp"

#include <iostream>
namespace advcpp
{
    
class Agent : public AbcAgent
{
public:
    Agent(EventsRouter& a_router, ServerHandler& a_server);

    //virtual void InitAgent(const std::string a_ip, const Config& a_config);
    virtual void Subscribe(Topic a_topic, SharedPtr<AbcAgent> a_pThis);
    virtual void Notify(const std::string& a_msg);
    virtual void Publish(const std::string& a_msg) const;
    virtual void Update(const std::string &a_msg) const ;
    //virtual void PushEvent();
    //virtual void Connect();
    //virtual void Stop();
    static int GetCounter() {return m_counter.Get();}
    static void ResetCounter() {m_counter.Reset();}
private:
    void fillEvent(const std::string& a_msg, Event& a_toFill) const ;

private: 
    static AtomicValue<int> m_counter;
};

} // namespace advcpp

#endif // E4AAC306_5CC5_433D_AC74_16234D1D2883_H__
