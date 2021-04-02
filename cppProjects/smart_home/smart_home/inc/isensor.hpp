#ifndef E7B4891C_A522_4227_A4C0_F33C3FDE3307_H__
#define E7B4891C_A522_4227_A4C0_F33C3FDE3307_H__

#include "noncopyable.hpp"

#include <string>

namespace advcpp
{
class ISensor{
public:
    virtual ~ISensor() = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0 ;
private:

};

inline ISensor::~ISensor()
{  
}

} // namespace advcpp

#endif // E7B4891C_A522_4227_A4C0_F33C3FDE3307_H__
