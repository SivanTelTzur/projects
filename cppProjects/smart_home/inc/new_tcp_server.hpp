#ifndef F7C7B7F5_95AE_4157_9A76_29F001F0D9C9_H__
#define F7C7B7F5_95AE_4157_9A76_29F001F0D9C9_H__

#include "inet_server.hpp"
#include "noncopyable.hpp"
#include "Imsg_handler_func.hpp"
#include "inew_client_action.hpp"
#include "sharedptr.hpp"

#include <string>
#include <list>
#include <sys/types.h> 
#include <tr1/unordered_map>

namespace advcpp
{

class TcpServer : private NonCopyable , public INetServer
{
    typedef SharedPtr<IMsgHandlerFunc> MsgFuncP;
    typedef SharedPtr<INewClientHandlerFunc> newClientFuncP;

public:
    enum {FAIL = -1, MAX_CLIENTS = 1020, MAX_LINE = 1024, BUFFER_SIZE = 256};
    TcpServer(size_t a_port, MsgFuncP a_getMsgFunction, newClientFuncP a_newClientFunc);
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
    int recvValue(int a_clientSocket);
    void recvMsg(int a_clientSocket, std::string& a_buffer);
    void clientReceive(fd_set& a_master, fd_set& a_temp, int a_activity);
    

private:
    int m_listenSock;
    const int m_port;
    std::list<int> m_clients;
    MsgFuncP m_getMsgFunction;
    newClientFuncP m_newClientFunc;
    std::tr1::unordered_map<int, std::string> m_ipMap;
};

class TcpServerLimitsException : public std::exception {
public:
    virtual const char* what() const throw();
};
} //advcpp

#endif // F7C7B7F5_95AE_4157_9A76_29F001F0D9C9_H__
