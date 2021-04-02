#ifndef AC98D696_0522_4961_BD72_9BAA8299DD6D_H__
#define AC98D696_0522_4961_BD72_9BAA8299DD6D_H__

#include "event.hpp"

namespace advcpp
{

class IRouter{
public:
    virtual ~IRouter() = 0;
    virtual void Notify(Event a_newEvent) = 0;
private:

};

inline IRouter::~IRouter()
{  
}     
    
} // namespace advcpp


#endif // AC98D696_0522_4961_BD72_9BAA8299DD6D_H__