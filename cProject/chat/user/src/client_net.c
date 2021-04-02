#include "client_net.h"
#include <sys/socket.h> /*socket*/
#include <stdio.h>      /*perror*/
#include <stdlib.h>
#include <netinet/in.h>/*inet_addr*/
#include <arpa/inet.h> /*inet_addr*/

#include <string.h> /*malloc , memset*/
#include <unistd.h> /*close*/

#define PORT 1200

/*TODO port and ip of server*/
int Client_Create()
{
    char strAddr[] = "127.0.0.1";
    struct sockaddr_in sin;
    int clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0)
    {
        perror("socket failed");
        return -1;
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(strAddr);
    sin.sin_port = htons(PORT);
    if (connect(clientsocket, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("connection failed");
    }
    return clientsocket;
}

int Send_Message(int pClient, char data_addr[], size_t data_len)
{
    ssize_t sent_bytes;
    sent_bytes = send(pClient, data_addr, data_len, 0);
    if (sent_bytes < 0)
    {
        /* perror("send failed"); */
        return 0;
    }
    return 1;
}

int Recv_Message(int pClient, char buffer[], size_t bufferLen)
{
    int read_byte = (int)recv(pClient, buffer, bufferLen, 0);
    if (read_byte == 0)
    {
        close(pClient);
        return 0;
    }
    else if (read_byte < 0)
    {
        perror("rcvfrom failed");
        close(pClient);
        return -1;
    }
    else
    {
        buffer[read_byte] = '\0';
        return 1;
    }
}
