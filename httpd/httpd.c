#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "netinet/in.h"
#include <arpa/inet.h>

#define LISTENADDR "0.0.0.0"

int initServer(int portNumber)
{
    int socketRet;
    struct sockaddr_in server;

    socketRet = socket(AF_INET, SOCK_STREAM, 0);
    if (socketRet < 0)
    {
        return 0;
    }

    server.sin_addr.s_addr = inet_addr(LISTENADDR);
    server.sin_family = AF_INET6;
    server.sin_port = htons(portNumber);
    
}

int main(int argc, char *argv[])
{
    int retServer;
    char* port;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
        return -1;
    }
    else
    {
        port = argv[1];
    }


    retServer = initServer(atoi(port));

    return 0;
}