#ifndef C054281E_77BB_46AD_BC9B_658DD37EEAE3_H__
#define C054281E_77BB_46AD_BC9B_658DD37EEAE3_H__

#include "iagent.hpp"
//#include "eventsRouter.hpp"
#include "irouter.hpp"
//#include "subscribersMng.hpp"
//#include "serverHandler.hpp"
#include "iserver.hpp"
#include "isubscription.hpp"

#include <iostream>
namespace advcpp
{
    
class TemperatureDemoAgent : public IAgent
{
public:
    TemperatureDemoAgent(IRouter& a_router, IServer& a_server, ISubscription& a_subscription);
    ~TemperatureDemoAgent();  

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
    void fillEvent(const std::string& a_msg, Event& a_toFill) const;
    char* PackEvent(unsigned int* a_buffersize, const Event a_event) const;

private: 
    static AtomicValue<int> m_counter;
    IServer& m_server;
    IRouter& m_router;
    ISubscription& m_subscription; 
    Config m_deviceData;
};


} // namespace advcpp

#endif // C054281E_77BB_46AD_BC9B_658DD37EEAE3_H__
