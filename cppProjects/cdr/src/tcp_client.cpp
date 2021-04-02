#include "tcp_client.hpp"
#include "sleep_micro_sec.hpp"
#include "logger.hpp"

#include <sys/socket.h> //socket
#include <sys/types.h> 
#include <unistd.h> //close
#include <netinet/in.h>//inet_addr, sockaddr_in
#include <arpa/inet.h> //inet_addr
#include <assert.h>
#include <errno.h>
#include <string.h>
//#include <exception>

namespace advcpp
{
TcpClient::TcpClient(const std::string& a_ip, size_t a_port) 
: m_socket(createSocket())
, m_ip(a_ip)
, m_port(a_port)
{
    try{
        connectSocket();
    }
    catch (const TcpConnectException& a_ex){
        StartLogger().Exception(a_ex);
        std::terminate();
    }    
}

TcpClient::~TcpClient() 
{
    close(m_socket);
}

void TcpClient::Send(const char* a_buffer) const
{
    //size_t buffLen = strlen(a_buffer) + 1;
    ssize_t sent_bytes = send(m_socket, a_buffer, BUFFER_SIZE, 0);
    if (sent_bytes < 0)
    {
       throw TcpSendFailException();
    }
}

int TcpClient::createSocket() 
{
    const int TRIES = 3;
    unsigned int sleepTimeUs = 80;
    for (int i = 0; i < TRIES; ++i){
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (0 <= m_socket){
            return m_socket;
        }
        switch (errno) {
            case EBADF:
                assert(!"file descriptor can't be valid at this point");
            case EACCES:
            case EAFNOSUPPORT:
            case EINVAL:
                assert(!"TcpClient - not possible errno");
            case EMFILE:
            case ENFILE:
            case ENOMEM:
            case ENOBUFS:
                SleepMicroSeconds(sleepTimeUs);
                sleepTimeUs *= sleepTimeUs;
                continue;
        }
    }     
    throw TcpLimitsexception();
}

void TcpClient::connectSocket() 
{
    struct sockaddr_in sin = sockaddr_in();
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(m_ip.c_str());
    sin.sin_port = htons(m_port);

    if (0 == connect(m_socket, reinterpret_cast<struct sockaddr *>(&sin), sizeof(sin)) ){
        return;
    }
    else{
        switch (errno){
            case EACCES:
            case EPERM:
            case EADDRINUSE:
            case EAFNOSUPPORT:
            case EAGAIN:
            case EALREADY:
            case EBADF:
            case EFAULT:
            case EISCONN:
            case ENOTSOCK:
            case EPROTOTYPE:
                assert(!"connect errno not possible with init args and exist socket");
            case ENETUNREACH:
            case ETIMEDOUT:
            case ECONNREFUSED:
                throw TcpConnectException();
        }
    }
}


const char* TcpLimitsexception::what() const throw()
{
    return "Tcp system limits has reached - socket failed";
}    

const char* TcpConnectException::what() const throw()
{
    return "Tcp connect - server errno ETIMEDOUT/ECONNREFUSED/ENETUNREACH ";
}    

const char* TcpSendFailException::what() const throw()
{
    return "Tcp send message failed";
} 
} // namespace advcpp
