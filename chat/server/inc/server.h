#ifndef __SERVER_H__
#define __SERVER_H__

/**
 *  @file server.h
 *  @brief Generic server select mode .
 *
 *  @details  Generic server, can handle 1000 max clients at once .
 *  GotMsgFunction must be given , other function can be NULL.
 *
 *  @author Sivan Tel-Tzur   (tz.sivan@gmail.com)
 *
 *  @bug No known bugs.
 */

typedef struct Server Server;

typedef enum Server_Result
{
    SERVER_OK = 0,
    SERVER_NULL_ARGUMENTS,     /**<Uninitialized server error */
    SERVER_INIT_FAIL,          /**<Initialized server failed */
    SERVER_OVERFLOW,           /**<max capacity of clients  */
    SERVER_ALLOCATION_FAIL,    /**<initial allocation or single sock allocation failed  */
    SERVER_CLIENT_CLOSED_PEER, /**<client closed peer  */
    SERVER_RECV_FAILED,        /**<receive failed  */
    SERVER_SEND_FAIL           /**<send data failed  */
} Server_Result;

typedef int (*GotMsgFunction)(char *_buffer, int _sock, int _size, void *_context);
typedef void (*NewClientFunction)(int _sock, void *_context);
typedef void (*CloseClientFunction)(const int _sock, void *_context);
typedef void (*FailFunction)(Server_Result _status, void *_context);

/**
 * @brief Create a new server , max clients is 1000
 * @param[in] _maxClients - Expected max capacity of clients (between 0-1000)
 * @param[in] _recvMsg - App function to get data received by client. Must be given to function
 * @param[in] _newClient - App function to alert about new client .
 * @param[in] _close - App function to alert about server closing a client .
 * @param[in] _fail - App function to alert about fail occurred .
 * @return newly created server
 */
Server *Server_Create(int _maxClients, GotMsgFunction _recvMsg, NewClientFunction _newClient, CloseClientFunction _close, FailFunction _fail, void *_context);

/**
 * @brief destroy server and all open sockets
 * @param[in] _server : server to be destroyed
 */
void DestroyServer(Server **_server);

/**
 * @brief activate and run the server
 * @param[in] _server - existing server
 * @retval No return value
 */
void Server_Run(Server *_server);

/**
 * @brief command the server to pass the client a message
 * @param[in] _data - data to transfer the client
 * @param[in] _size - size of data
 * @param[in] _client - client socket
 * @return return -1 on fail or 1 for success
 * @retval  return -1 on fail or 1 for success
 */
Server_Result Server_Send_Message(char *_data, int _size, int _client);

#endif /*  __SERVER_H__ */
