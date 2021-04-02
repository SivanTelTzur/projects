#ifndef BDFA25A3_3FED_47CD_9DE4_6114C292589C_H__
#define BDFA25A3_3FED_47CD_9DE4_6114C292589C_H__

#include "inet_server.hpp"
#include "noncopyable.hpp"
#include "waiting_queue.hpp"

#include <string>
#include <list>
#include <sys/types.h> 

namespace advcpp
{

class TcpServer : private NonCopyable , public INetServer
{
public:
    enum {FAIL = -1, MAX_CLIENTS = 1020, MAX_LINE = 1024, BUFFER_SIZE = 256};
    TcpServer(size_t a_port, MtWaitingQueue<std::string>& a_msgQueue);
    ~TcpServer();

    virtual void Run();
    virtual void Receive(char* a_buffer);
    virtual void Send(const char* a_buffer, size_t a_size, int a_client);

private:
    int createSocket();
    void noBlockinSock();
    void setReuseSockOpt();
    void bindSocket();
    void listenSock();
    void runServer();
    void createNewClient(fd_set& a_master);
    void clientReceive(fd_set& a_master, fd_set& a_temp, int a_activity);
    int recvValue(int a_clientSocket);
    void recvMsg(int a_clientSocket, std::string& a_buffer);

private:
    int m_listenSock;
    const int m_port;
    std::list<int> m_clients;
    MtWaitingQueue<std::string>& m_msg;
    
};

class TcpServerLimitsException : public std::exception {
public:
    virtual const char* what() const throw();
};
} //advcpp

#endif // BDFA25A3_3FED_47CD_9DE4_6114C292589C_H__
