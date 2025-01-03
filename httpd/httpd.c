#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "netinet/in.h"
#include <arpa/inet.h>

#define LISTENADDR "0.0.0.0"

char* error;

int initServer(int portNumber)
{
    int socketFd;
    struct sockaddr_in server;

    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
    {
        error = "scoket() errro";
        return 0;
    }

    server.sin_addr.s_addr = inet_addr(LISTENADDR);
    server.sin_family = AF_INET6;
    server.sin_port = htons(portNumber);

    if (bind(socketFd, (struct socketaddr*)&server, sizeof(server)))
    {
        error = "bind() eroor";
        close(socketFd);
        return 0;
    }
    
    if (listen(socketFd, 5))
    {
        error = "listen() error";
        close(socketFd);
        return 0;
    }
    
    return socketFd;
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
    if (!retServer)
    {
        fprintf(stderr, "%s\n", error);
        return -1;
    }
    

    return 0;
}