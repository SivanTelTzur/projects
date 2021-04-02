
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>     /*memset*/
#include <sys/types.h>  /*bind, fd_set*/
#include <sys/socket.h> /*socket*/
#include <arpa/inet.h>  /*htons*/
#include <fcntl.h>      /*fcntl*/
#include <errno.h>      /*errno*/
#include <sys/select.h> /* select */
#include <sys/time.h>   /* fd_set */
#include "server.h"
#include "list_itr.h"

struct Server
{
    GotMsgFunction m_appGotMsg;
    NewClientFunction m_appNewClient;
    CloseClientFunction m_appCloseClient;
    FailFunction m_appFail;
    List *m_clients;
    int m_listenSock;
    int m_nMaxClients;
    int m_nOfClients;
    fd_set m_master;
    void *m_context;
};

#define PORT 1200
#define MAXLINE 1024
#define MAX_OF_CLIENTS 1000
#define TRUE 1
#define FALSE 0
#define FAIL -1
#define CLOSE -1
#define OK 1
#define GET_N_CLIENTS 5

static int Init_Server(int _queueSize);
static void ClientsRcv(Server *_server, fd_set *_temp, int _activity);
static void CheckNewClient(Server *_server);
static int CheckCurrentClient(Server *_server, int _clientSock);
static int NoBlockinSock(int _socket);
static void DestroySock(void *_clientSock);
static void SendFail(FailFunction _fail, Server_Result _failStatus, void *_context);

Server *Server_Create(int _maxClients, GotMsgFunction _recvMsg, NewClientFunction _newClient, CloseClientFunction _close, FailFunction _fail, void *_context)
{
    int sockListen;
    List *clients = NULL;
    Server *_newServer = NULL;

    if (_maxClients <= 0 || _maxClients > MAX_OF_CLIENTS || _recvMsg == NULL)
    {
        SendFail(_fail, SERVER_NULL_ARGUMENTS, _context);
        return NULL;
    }
    if (FAIL == (sockListen = Init_Server(_maxClients)))
    {
        SendFail(_fail, SERVER_INIT_FAIL, _context);
        return NULL;
    }

    if ((clients = List_Create()) == NULL)
    {
        SendFail(_fail, SERVER_ALLOCATION_FAIL, _context);
        return NULL;
    }

    _newServer = malloc(sizeof(Server));
    if (_newServer == NULL)
    {
        SendFail(_fail, SERVER_ALLOCATION_FAIL, _context);
        List_Destroy(&clients, NULL);
    }

    _newServer->m_appGotMsg = _recvMsg;
    _newServer->m_appNewClient = _newClient;
    _newServer->m_appCloseClient = _close;
    _newServer->m_appFail = _fail;
    _newServer->m_clients = clients;
    _newServer->m_listenSock = sockListen;
    _newServer->m_nMaxClients = _maxClients;
    _newServer->m_nOfClients = 0;
    _newServer->m_context = _context;
    FD_ZERO(&_newServer->m_master);
    FD_SET(sockListen, &_newServer->m_master);

    return _newServer;
}

void Server_Run(Server *_server)
{
    fd_set temp;
    int activity = 0;

    FD_ZERO(&temp);

    while (TRUE)
    {
        temp = _server->m_master;
        activity = select(MAXLINE, &temp, NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR)
        {
            break;
        }

        /* New client check */
        if (activity > 0 && FD_ISSET(_server->m_listenSock, &temp))
        {
            CheckNewClient(_server);
            --activity;
        }

        if (activity > 0)
        {
            ClientsRcv(_server, &temp, activity);
        }
        activity = 0;
    }
}

Server_Result Server_Send_Message(char *_data, int _size, int _client)
{
    ssize_t sent_byte;

    sent_byte = send(_client, _data, (size_t)_size, 0);
    if (sent_byte < 0)
    {
        return SERVER_SEND_FAIL;
    }
    return SERVER_OK;
}

void DestroyServer(Server **_server)
{
    if (_server == NULL || *_server == NULL)
    {
        return;
    }
    close((*_server)->m_listenSock);
    List_Destroy(&(*_server)->m_clients, DestroySock);
    *_server = NULL;
}

/***************************************************************************************************************/

static void SendFail(FailFunction _fail, Server_Result _failStatus, void *_context)
{
    if (_fail)
    {
        _fail(_failStatus, _context);
    }
}

static void ClientsRcv(Server *_server, fd_set *_temp, int _activity)
{
    ListItr currItr = NULL, endItr = NULL;
    int currClientSock;
    int *deleteClient = NULL;

    currItr = ListItr_Begin(_server->m_clients);
    endItr = ListItr_End(_server->m_clients);

    while (_activity > 0 && currItr != endItr)
    {
        currClientSock = *((int *)(ListItr_Get(currItr)));

        currItr = ListItr_Next(currItr);
        if (FD_ISSET(currClientSock, _temp))
        {
            if (CLOSE == CheckCurrentClient(_server, currClientSock))
            {
                close(currClientSock);
                --_server->m_nOfClients;
                FD_CLR(currClientSock, &_server->m_master);
                if (_server->m_appCloseClient)
                {
                    _server->m_appCloseClient(currClientSock, _server->m_context);
                }
                deleteClient = (int *)ListItr_Remove(ListItr_Prev(currItr));
                free(deleteClient);
            }
            --_activity;
        }
    }
}

static int NoBlockinSock(int _socket)
{
    int flags;

    if (FAIL == (flags = fcntl(_socket, F_GETFL)))
    {
        perror("error at fcntl. F_GETFL.");
        return _socket;
    }
    if (FAIL == fcntl(_socket, F_SETFL, flags | O_NONBLOCK))
    {
        perror("error at fcntl. F_SETFL.");
    }
    return _socket;
}

static int Init_Server(int _queueSize)
{
    struct sockaddr_in sin;
    int sockListen, optval = 1;

    sockListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sockListen < 0)
    {
        perror("socket failed");
        return FAIL;
    }
    NoBlockinSock(sockListen);

    if (setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        perror("reuse error");
        return FAIL;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);

    if (bind(sockListen, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("bind failed");
        return FAIL;
    }

    if (listen(sockListen, _queueSize))
    {
        perror("listen error");
        return FAIL;
    }
    return sockListen;
}

static void CheckNewClient(Server *_server)
{
    struct sockaddr_in client_sin;
    int clientSock, i;
    unsigned int addr_sin = sizeof(client_sin);
    int *pCurrClient = NULL;

    for (i = 0; i < GET_N_CLIENTS; ++i)
    {
        clientSock = accept(_server->m_listenSock, (struct sockaddr *)&client_sin, &addr_sin);

        if (_server->m_nOfClients >= _server->m_nMaxClients || clientSock >= MAXLINE)
        {
            close(clientSock);
            SendFail(_server->m_appFail, SERVER_OVERFLOW, _server->m_context);
            return;
        }
        if (clientSock < 0)
        {
            close(clientSock);
            return;
        }
        if ((pCurrClient = (int *)malloc(sizeof(int))) == NULL)
        {
            SendFail(_server->m_appFail, SERVER_ALLOCATION_FAIL, _server->m_context);
            return;
        }
        *pCurrClient = clientSock;
        _server->m_appNewClient(clientSock, _server->m_context);
        ++_server->m_nOfClients;
        FD_SET(clientSock, &_server->m_master);
        List_PushHead(_server->m_clients, pCurrClient);
    }
}

static int CheckCurrentClient(Server *_server, int _clientSock)
{
    ssize_t read_byte;
    char buffer[256];

    read_byte = recv(_clientSock, buffer, sizeof(buffer), 0);

    if (read_byte == 0)
    {
        SendFail(_server->m_appFail, SERVER_CLIENT_CLOSED_PEER, _server->m_context);
        return CLOSE;
    }
    else if (read_byte < 0)
    {
        SendFail(_server->m_appFail, SERVER_RECV_FAILED, _server->m_context);
        return CLOSE;
    }
    else
    {
        _server->m_appGotMsg(buffer, _clientSock, (int)read_byte, _server->m_context);
    }
    return OK;
}

void DestroySock(void *_clientSock)
{
    close(*((int *)_clientSock));
    free((int *)_clientSock);
}
