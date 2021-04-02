#ifndef FEBA9D4F_81C3_4C01_A40D_FA657C0DB3D7_H__
#define FEBA9D4F_81C3_4C01_A40D_FA657C0DB3D7_H__

#include "config_struct.hpp"
#include "event.hpp"
#include "sharedptr.hpp"

#include <string>

namespace advcpp
{
    
class IAgent
{
public:
    virtual ~IAgent() = 0;
    virtual void testMe()const = 0;

    virtual void InitAgent(const Config& a_config, SharedPtr<IAgent> a_pThis) = 0;
    virtual void Subscribe(Topic a_topic, SharedPtr<IAgent> a_pThis) = 0;

    //Event received from the sensor device
    virtual void Notify(const std::string &a_msg) = 0;

    //Update a device upon it's subscription
    virtual void Update(const Event a_event) const = 0;
    virtual void UpdateSocket(int a_socketUpdate) = 0;

    virtual const Config& GetDeviceData() const = 0;
};

inline IAgent::~IAgent()
{
    
}


} // namespace advcpp

#endif // FEBA9D4F_81C3_4C01_A40D_FA657C0DB3D7_H__