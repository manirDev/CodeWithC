#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_PORT_NUMBER 4321
#define BUFFER_SIZE 0x200


int main()
{
    int retServer, retClient;
    struct sockaddr_in server, client;
    int addrLen = 0;
    char buffer[BUFFER_SIZE];
    char* data;

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    retServer = socket(AF_INET, SOCK_STREAM, 0);
    if (retServer < 0)
    {
        printf("socket() error\n");
        close(retServer);
        return 0;
    }

    printf("Server is initiated\n");
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT_NUMBER);

    int isBind = bind(retServer, (struct sockaddr *)&server, sizeof(server));
    if (isBind < 0)
    {
        printf("bind() error\n");
        close(retServer);
        return 0;
    }
    printf("Server is binded\n");
    
    int isListen = listen(retServer, 5);
    if (isListen < 0)
    {
        printf("listen() erro\n");
        close(retServer);
        return 0;
    }
    printf("Listen started\n");
    
    retClient = accept(retServer, (struct sockaddr*)&server, &addrLen);
    if (retClient < 0)
    {
        printf("accept() error\n");
        close(retServer);
        return 0;
    }
    
    printf("Client connected\n");

    read(retClient, buffer, BUFFER_SIZE - 1);
    data = "httpd v1.0\n";
    write(retClient, data, strlen(data));

    close(retClient);
    close(retServer);
    return 0;
}