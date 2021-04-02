#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* for sleep() & ppid */

#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
    char msgbuf[MSGBUFSIZE];
    const char s[2] = ":";
    char *token;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int nbytes;
    struct ip_mreq mreq;
    FILE* fptr = fopen("pid2.txt","w");
    int yes = 1;
    int id = getpid();
    int fd, port;
    char* groupIP;
    fprintf(fptr,"%d",id);
    fclose(fptr);
       
    if (argc != 4) /*  basic + ip + port + groupName TODO keep groupName */
    { 
       return 1;
    }
    groupIP = argv[1]; /*  IP ADDRESS XXX.XXX.XXX.XXX */
    port = atoi(argv[2]); /*  0 if error, which is an invalid port */
    /* create what looks like an ordinary UDP socket */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) 
    {
        perror("socket");
        return 1;
    }
    printf("WELCOME TO GROUP %s\n\n",argv[3]);

    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0)
    {
       perror("Reusing ADDR failed");
       return 1;
    }

    /* set up destination address */

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    
    addr.sin_addr.s_addr = inet_addr(argv[1]); /* FIX BUG LINE - work only for specific IP before fix "INADOOR_ANY"*/
    addr.sin_port = htons((u_int16_t)port);
      

    /* bind to receive address */
    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) 
    {
        perror("bind");
        return 1;
    }

    /* use setsockopt() to request that the kernel join a multicast groupIP */
    mreq.imr_multiaddr.s_addr = inet_addr(groupIP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt");
        return 1;
    }

    /* now just enter a read-print loop */
   

    while (1) 
    {
        nbytes = (int)recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *) &addr, &addrlen);
        if (nbytes < 0) 
        {
            perror("recvfrom");
            return 1;
        }
        token = strtok(msgbuf,s);
        printf("\033[1;36m");
        printf("%s:",token);
        token = strtok(NULL,s);
        printf("\033[1;33m");
        printf(" %s\n",token);
       
      
     }
    return 0;
}