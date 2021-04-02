#ifndef H0CE1994_1147_4C6E_B9AB_05117CCD360B_H__
#define H0CE1994_1147_4C6E_B9AB_05117CCD360B_H__

#include <unistd.h> //size_t
namespace advcpp
{
class INetServer
{
public:
    virtual ~INetServer() = 0;

    virtual void Run() = 0;
    virtual void Receive(char* a_buffer) = 0;
    virtual void Send(const char* a_buffer, size_t a_size, int a_client) = 0;
private:

};

inline INetServer::~INetServer()
{
    
}

} // namespace advcpp

#endif // H0CE1994_1147_4C6E_B9AB_05117CCD360B_H__