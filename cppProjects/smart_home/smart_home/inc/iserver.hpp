#ifndef E0EBF738_8693_4F7D_ADC2_28EF0513D88E_H__
#define E0EBF738_8693_4F7D_ADC2_28EF0513D88E_H__

#include <cstdlib> // size_t

namespace advcpp
{

class IServer{
public:
    virtual ~IServer() = 0;
    virtual void Send(const char* a_buffer, size_t a_size, int a_client) = 0;
private:

};

inline IServer::~IServer()
{  
}     
    
} // namespace advcpp

#endif // E0EBF738_8693_4F7D_ADC2_28EF0513D88E_H__
