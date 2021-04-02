#ifndef F99EEF8C_589B_474D_A422_C9589ADF5742_H__
#define F99EEF8C_589B_474D_A422_C9589ADF5742_H__

#include "iagent.hpp"

namespace advcpp
{

class ISubscription{
public:
    virtual ~ISubscription() = 0;
    virtual void Subscribe(const Topic a_topic, SharedPtr<IAgent> a_Agent) = 0;
private:

};

inline ISubscription::~ISubscription()
{  
}     
    
} // namespace advcpp

#endif // F99EEF8C_589B_474D_A422_C9589ADF5742_H__