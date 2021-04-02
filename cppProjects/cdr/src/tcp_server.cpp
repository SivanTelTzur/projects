#include "tcp_server.hpp"
#include "sleep_micro_sec.hpp"
#include "logger.hpp"
//#include "protocol.hpp"

#include <sys/socket.h> //socket
#include <unistd.h> //close
#include <netinet/in.h>//inet_addr, sockaddr_in
#include <arpa/inet.h> //inet_addr htons
#include <fcntl.h> //fcntl
#include <sys/select.h> // select 
#include <sys/time.h>   // fd_set 
#include <assert.h>
#include <string.h> //memset DELETE
#include <errno.h>
#include <iostream> //DELETE

namespace advcpp
{
TcpServer::TcpServer(size_t a_port, MtWaitingQueue<std::string>& a_msgQueue) 
: m_listenSock(createSocket())
, m_port(a_port)
, m_clients()
, m_msg(a_msgQueue)
{
    noBlockinSock();
    setReuseSockOpt();
    bindSocket();
    listenSock();    
}

TcpServer::~TcpServer() 
{
    std::list<int>::iterator it = m_clients.begin();
    while (it != m_clients.end()){
        close(*it);
        ++it;
    }
    close(m_listenSock);
}

void TcpServer::Run() 
{
    runServer();  
}

void TcpServer::Receive(char* a_buffer) 
{
    
}

void TcpServer::Send(const char* a_buffer, size_t a_size, int a_client) 
{
    ssize_t sent_byte = send(a_client, a_buffer, a_size, 0);
    if (sent_byte < 0)
    {
        StartLogger().Message("server - send fail");
    }
}   


int TcpServer::createSocket() 
{
    const int TRIES = 3;
    unsigned int sleepTimeUs = 80;
    for (int i = 0; i < TRIES; ++i){
        m_listenSock = socket(AF_INET, SOCK_STREAM, 0);
        if (0 <= m_listenSock){
            return m_listenSock;
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
    throw TcpServerLimitsException();    
}

void TcpServer::noBlockinSock() 
{
    int flags = fcntl(m_listenSock, F_GETFL);
    if (FAIL == flags)
    {
        StartLogger().Message("server - no blocking failed");
        std::terminate();
    }
    if (FAIL == fcntl(m_listenSock, F_SETFL, flags | O_NONBLOCK))
    {
        StartLogger().Message("server - no blocking failed");
        std::terminate();
    }   
}

void TcpServer::setReuseSockOpt() 
{
    int optval = 1;

    if (setsockopt(m_listenSock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        StartLogger().Message("server - setSockOpt -  - reusedfailed\n");
        std::terminate();
    }    
}

void TcpServer::bindSocket() 
{
    struct sockaddr_in sin = sockaddr_in();
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(m_port);

    if (bind(m_listenSock, reinterpret_cast<struct sockaddr *>(&sin), sizeof(sin)) < 0)
    {
        StartLogger().Message("server - bindSocket - bind failed\n");
        std::terminate();
    }    
}

void TcpServer::listenSock() 
{
    if (listen(m_listenSock, MAX_CLIENTS))
    {
        StartLogger().Message("server - listenSock - listen failed\n");
        std::terminate();
    }    
}

void TcpServer::runServer() 
{
    fd_set master;
    FD_ZERO(&master);
    FD_SET(m_listenSock, &master);

    while(true)
    {
        fd_set temp;
        FD_ZERO(&temp);
        temp = master;
        int activity = 0;
        
        activity = select(MAX_LINE, &temp, NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR)
        {
            StartLogger().Message("server - select failed\n");
            std::terminate();
        }

        /// New client check
        if (activity > 0 && FD_ISSET(m_listenSock, &temp))
        {
            createNewClient(master);
            --activity;
        }

        if (activity > 0)
        {
            clientReceive(master, temp, activity);
        }
    }
}

void TcpServer::createNewClient(fd_set& a_master) 
{
    int clientSock;
    struct sockaddr_in client_sin;
    unsigned int addr_sin = sizeof(client_sin);
    clientSock = accept(m_listenSock, (struct sockaddr *)&client_sin, &addr_sin);

    if (m_clients.size() >= MAX_CLIENTS || clientSock >= MAX_LINE)
    {
        close(clientSock);
        StartLogger().Message("server overflow can't get new clients\n");
        return;
    }

    if (clientSock < 0)
    {
        close(clientSock);
        StartLogger().Message("server accept failed\n");
        return;
    }
    FD_SET(clientSock, &a_master);
    m_clients.push_back(clientSock);

}

void TcpServer::clientReceive(fd_set& a_master, fd_set& a_temp, int a_activity) 
{

    std::list<int>::iterator it = m_clients.begin();
    while (it != m_clients.end() && a_activity > 0){
        int currClientSock = *it;

        if (FD_ISSET(currClientSock, &a_temp)){
            char buffer[256];
            ssize_t read_byte = recv(currClientSock, buffer, sizeof(buffer), 0);
            StartLogger().Message(buffer);

            if (0 >= read_byte){
                close(currClientSock);
                FD_CLR(currClientSock, &a_master);
                it = m_clients.erase(it);
                StartLogger().Message("client closed - message was not received \n");
            }else{
                m_msg.Enqueue(buffer);
                ++it;
            }            
            --a_activity;
        }else{
            ++it;
        }
    }
}


const char* TcpServerLimitsException::what() const throw()
{
    return "Tcp server system limits has reached - socket failed";
} 


} //advcpp

