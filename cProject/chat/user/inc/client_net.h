#ifndef __CLIENT_NET_H__
#define __CLIENT_NET_H__
#include <stddef.h>  /* size_t */
typedef struct Client Client;

/**  
 * @brief creates a socket and connect it to the server. 
 * @return the number of the socket 
 *
 * @warning if connection to the server fails it closed the program
 */
int Client_Create();

/**  
 * @brief Sends message to the server  
 * @param[in] socket number , buffer ,  size of the buffer.
 * @param[in] buffer with the message to send.
 * @param[in] size of the buffer.
 * @return return 1 for successfull send , 0  fails to send to server
 * */
int Send_Message(int pClient,char data_addr[],size_t data_len);

/**  
 * @brief Recieve message from the server.  
 * @param[in] socket number , buffer ,  size of the buffer.
 * @param[in] buffer to recieve the message, NOTE:need to be at least as size of the recieved message.
 * @param[in] size of the buffer.
 * @return return 1 for successfull recieve , 0  socket closed, -1 recieved failed.
 * */
int Recv_Message(int pClient,char buffer[],size_t bufferLen);
#endif
















typedef int (*newClient)(int);
typedef int (*newClientFail)(int);
typedef int (*gotMessage)(int,char[],size_t);
typedef int (*closeClient)(int);

