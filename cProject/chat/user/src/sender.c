#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  /* for sleep() & ppid */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MESSAGE_SIZE 256

int main(int argc, char *argv[])
{
    char message [MESSAGE_SIZE];
    struct sockaddr_in addr;
    int fd;
    int port;
    int nbytes;
    char* group;
    char* userName;
    FILE* fptr = fopen("pid.txt","w");
    int id = getpid();
    fprintf(fptr,"%d",id);
    fclose(fptr);

    if (argc != 5) /*  basic + ip + port+ userName + groupName TODO keep groupName*/
    {
       return 1;
    }

    group = argv[1]; /*  IP ADDRESS XXX.XXX.XXX.XXX */
    port = atoi(argv[2]); /*  0 if error, which is an invalid port */
    userName = argv[3];
    printf("WELCOME TO GROUP %s\n\n",argv[4]);

   
    /* create what looks like an ordinary UDP socket */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) 
    {
        perror("socket");
        return 1;
    }

    /* set up destination address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons((uint16_t)port);

    /* now just sendto() our destination! */
    while (1) 
    {
        char fullMessage[MESSAGE_SIZE] = {'\0'};
        printf("%s: ",userName);
        scanf("%230[^\n]",message);/* controlling length */
        getchar();
        strcpy(fullMessage,userName);
        strcat(fullMessage," : ");
        strcat(fullMessage,message);
        nbytes = (int)sendto(fd, fullMessage, strlen(fullMessage)+1, 0,(struct sockaddr*) &addr, sizeof(addr));
        if (nbytes < 0) 
        {
            perror("sendto");
            return 1;
        }
       

     }
    return 0;
}