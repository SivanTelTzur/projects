#ifndef F2D7739B_3927_4E2D_9A82_5944DF135047_H__
#define F2D7739B_3927_4E2D_9A82_5944DF135047_H__

#include "inet_client.hpp"

#include <string>

namespace advcpp
{

class Controller{
public:
    Controller(const std::string& a_ip, size_t a_port);
    ~Controller();
    void Run();
    void Stop();
private:
    void recvData(std::string a_msg);

private:
    INetClient* m_net;
};
    
} // namespace advcpp

#endif // F2D7739B_3927_4E2D_9A82_5944DF135047_H__
