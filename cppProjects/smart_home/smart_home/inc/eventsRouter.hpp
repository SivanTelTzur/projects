#ifndef D4DFB006_72C7_4F04_A36C_111F34B16AA1_H__
#define D4DFB006_72C7_4F04_A36C_111F34B16AA1_H__

#include "irouter.hpp"
#include "noncopyable.hpp"
#include "event.hpp"
#include "iagent.hpp"
#include "waitableQueue.hpp"
#include "subscribersMng.hpp"

namespace advcpp
{

class IAgent;    

class EventsRouter : public IRouter, private NonCopyable
{ 
public:
    explicit EventsRouter(SubscribersMng& a_subscribers);
    ~EventsRouter();

    void Run();

    virtual void Notify(Event a_newEvent);

private:
    void updateSubscribers(Event a_newEvent);

private:
    SubscribersMng& m_subscribers;
    WaitableQueue<Event> m_events;
};
    
} // namespace advcpp


#endif // D4DFB006_72C7_4F04_A36C_111F34B16AA1_H__