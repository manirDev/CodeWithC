#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

//Note: this is google.se ip address.
#define IP "142.250.187.163"
#define PORT 80
#define BUFFER_SIZE 0x200

int main()
{
    int ret;
    struct sockaddr_in client;
    char buffer[BUFFER_SIZE];
    char* data = "HEAD / HTTP/1.0\n\n";

    ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret < 0)
    {
        printf("socket() error");
        return 0;
    }
    
    client.sin_addr.s_addr = inet_addr(IP);
    client.sin_port = htons(PORT);
    client.sin_family = AF_INET;

    int connection = connect(ret,  (struct sockaddr*)&client, sizeof(struct sockaddr_in));
    if (connection != 0)
    {
        printf("connect() error\n");
        close(ret);
    }

    write(ret, data, strlen(data));
    memset(buffer, 0, BUFFER_SIZE);
    read(ret, buffer, BUFFER_SIZE - 1);  
    close(ret);

    printf("\n%s\n", buffer);

    return 0;
}