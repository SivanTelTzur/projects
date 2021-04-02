#ifndef DF15DBE0_AB6A_4B68_9F14_549866B770D9_H__
#define DF15DBE0_AB6A_4B68_9F14_549866B770D9_H__

#include "noncopyable.hpp"

#include <sys/socket.h> /*socket*/
#include <string>

namespace advcpp
{
class ClientNet : private NonCopyable
{
public:
    static const char Inet_addr[10];
    static const int Port;

    ClientNet();
    //~ClientNet = default ? need to close

    void SendMsg(const std::string& a_str);
    std::string GetMsg();

private:
    //reconnect ??? socket may be closed
private:
    sockaddr_in m_socketAddr;
    int m_socket;
}

} // namespace advcpp



#endif // !DF15DBE0_AB6A_4B68_9F14_549866B770D9_H__