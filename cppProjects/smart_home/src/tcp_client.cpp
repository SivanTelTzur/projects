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
        bindSocket();
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

void TcpClient::Send(const char* a_buffer, unsigned int a_len) const
{
    ssize_t sent_bytes = send(m_socket, a_buffer, a_len, 0);
    std::cerr << "rcp client send a_len" << a_len << std::endl;
    if (sent_bytes < 0)     
    {
       throw TcpSendFailException();
    }
}

void TcpClient::Recv(std::string& a_BufferToFill) const
{
    try{
        recvMsg(a_BufferToFill);        
    }
    catch(const std::runtime_error& a_ex){
        StartLogger().Exception(a_ex);
        StartLogger().Message("receive msg from server failed");
        std::terminate();
    }   
}

void TcpClient::recvMsg(std::string& a_buffer) const
{
    a_buffer.clear();
    int bytesToRecv = 0;
    try{
        bytesToRecv = recvValue();
    }
    catch (const std::runtime_error& a_ex){
        throw;
    }
    char * buffer = new char[bytesToRecv];
    while (bytesToRecv){
        ssize_t recv_bytes = recv(m_socket, buffer, bytesToRecv, 0);
        if (recv_bytes <= 0){
            delete buffer;
            throw std::runtime_error("recv failed");  
        }
        a_buffer.append(buffer, recv_bytes);
        bytesToRecv -= recv_bytes;
    }   
    delete buffer;
}


int TcpClient::recvValue() const
{
    int toRead = 4;
    char valueLen[4];
    char * buffer = valueLen; 

    while (toRead > 0){
        ssize_t recv_bytes = recv(m_socket, buffer, toRead, 0);
        if (recv_bytes <= 0 ){
            throw   std::runtime_error("recv failed");
        }
        toRead -= recv_bytes; // read less next time 
        buffer += recv_bytes; //move buffer to next place
    }
        //return  *((int*)valueLen); 
    return ntohl(*(reinterpret_cast<int*>(valueLen)));
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
    sin.sin_addr.s_addr = inet_addr("127.8.8.1");
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

void TcpClient::bindSocket() 
{
    int optval = 1;

    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        StartLogger().Message("client - setSockOpt -  - reusedfailed\n");
        std::terminate();
    }  
    struct sockaddr_in sinBind = sockaddr_in();
    memset(&sinBind, 0, sizeof(sinBind));
    sinBind.sin_family = AF_INET;
    sinBind.sin_addr.s_addr = inet_addr(m_ip.c_str());//INADDR_ANY;
    sinBind.sin_port = htons(m_port);

    if (bind(m_socket, reinterpret_cast<struct sockaddr *>(&sinBind), sizeof(sinBind)) < 0)
    {
        StartLogger().Message("server - bindSocket - bind failed\n");
        std::terminate();
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
