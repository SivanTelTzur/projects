#ifndef DA3B40E2_827F_4C05_99E9_5F57CFFE95C6_H__
#define DA3B40E2_827F_4C05_99E9_5F57CFFE95C6_H__

#include "inet_client.hpp"
#include "noncopyable.hpp"
#include <string>

namespace advcpp
{
class TcpClient : private NonCopyable , public INetClient
{
public:
    enum {BUFFER_SIZE = 256};
    TcpClient(const std::string& a_ip, size_t a_port);
    ~TcpClient();

    virtual void Send(const char* a_buffer, unsigned int a_len) const;
    virtual void Recv(std::string& a_BufferToFill) const;

private:
    int createSocket();
    void connectSocket();
    void bindSocket();
    int recvValue() const;
    void recvMsg(std::string& a_buffer) const;

private:
    int m_socket;
    const std::string m_ip;
    int m_port;
};


class TcpLimitsexception : public std::exception {
public:
    virtual const char* what() const throw();
};


class TcpConnectException : public std::exception {
public:
    virtual const char* what() const throw();
};

class TcpSendFailException : public std::exception {
public:
    virtual const char* what() const throw();
};

} // namespace advcpp

#endif // DA3B40E2_827F_4C05_99E9_5F57CFFE95C6_H__