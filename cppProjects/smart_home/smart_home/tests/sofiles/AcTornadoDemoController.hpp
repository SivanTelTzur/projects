#ifndef BBB6C7BD_46AA_42EA_A87C_A7FA062D029C_H__
#define BBB6C7BD_46AA_42EA_A87C_A7FA062D029C_H__

#include "iagent.hpp"
//#include "eventsRouter.hpp"
//#include "subscribersMng.hpp"
//#include "serverHandler.hpp"
#include "isubscription.hpp"
#include "iserver.hpp"
#include "irouter.hpp"

#include <iostream>
namespace advcpp
{
    
class AcTornadoDemoController : public IAgent
{
public:
    AcTornadoDemoController(IRouter& a_router, IServer& a_server, ISubscription& a_subscription);
    ~AcTornadoDemoController();  

    virtual void InitAgent(const Config& a_config, SharedPtr<IAgent> a_pThis);
    virtual void Subscribe(Topic a_topic, SharedPtr<IAgent> a_pThis);

    virtual void Notify(const std::string& a_msg);

    //tests func
    virtual void testMe()const;

    virtual void Update(const Event a_event) const ;
    virtual void UpdateSocket(int a_socketUpdate);

    static int GetCounter() {return m_counter.Get();}
    static void ResetCounter() {m_counter.Reset();}

    virtual const Config& GetDeviceData() const {return m_deviceData;}

private:
    //void fillEvent(const std::string& a_msg, Event& a_toFill) const;
    char* PackEvent(unsigned int* a_buffersize, const Event a_event) const;
    void subscribeEventsFromConfig(SharedPtr<IAgent> a_pThis);

private: 
    static AtomicValue<int> m_counter;
    IServer& m_server;
    IRouter& m_router;
    ISubscription& m_subscription; 
    Config m_deviceData;
};


} // namespace advcpp

#endif // BBB6C7BD_46AA_42EA_A87C_A7FA062D029C_H__
