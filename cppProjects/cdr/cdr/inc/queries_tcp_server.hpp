#ifndef AABD89FD_2470_45C3_AD1E_240D37C8C911_H__
#define AABD89FD_2470_45C3_AD1E_240D37C8C911_H__

#include "inet_server.hpp"
#include "noncopyable.hpp"
#include "waiting_queue.hpp"
#include "query.hpp"

#include <list>
#include <sys/types.h> 

namespace advcpp
{

class QueriesTcpServer : private NonCopyable , public INetServer
{
public:
    enum {FAIL = -1, MAX_CLIENTS = 1020, MAX_LINE = 1024};
    QueriesTcpServer(size_t a_port, MtWaitingQueue<Query>& a_msgQueue);
    ~QueriesTcpServer();

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

private:
    int m_listenSock;
    const int m_port;
    std::list<int> m_clients;
    MtWaitingQueue<Query>& m_msg;
    
};

class QueriesServerLimitsException : public std::exception {
public:
    virtual const char* what() const throw();
};
} //advcpp

#endif // AABD89FD_2470_45C3_AD1E_240D37C8C911_H__
