#include "httpd.h"

char *error;

const char *getContentType(const char *filePath)
{
    if (strstr(filePath, ".jpg") || strstr(filePath, ".jpeg"))
        return "image/jpeg";
    if (strstr(filePath, ".png"))
        return "image/png";
    if (strstr(filePath, ".html"))
        return "text/html";
    return "application/octet-stream";
}

char *loadFileContent(const char *filePath, int *fileSize)
{
    printf("\n%s\n", filePath);
    FILE *fp = fopen(filePath, "rb");
    if (NULL == fp)
    {
        error = "fopen() error";
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *fileSize = ftell(fp);
    rewind(fp);

    char *content = malloc(*fileSize);
    if (NULL == content)
    {
        error = "malloc() error";
        fclose(fp);
        return NULL;
    }

    fread(content, 1, *fileSize, fp);
    printf("\n%s\n", content);
    fclose(fp);

    return content;
}

void httpHeaders(int retClient, int responseCode, char *contentType, int contentLength)
{
    char buff[REQ_BUFFER_SIZE];
    snprintf(buff, sizeof(buff) - 1,
             "HTTP/1.0 %d OK\r\n"
             "Server: httpd.c\r\n"
             "Cache-Control: no-store, no-cache, max-age=0, private\r\n"
             "Content-Language: en\r\n"
             "Expires: -1\r\n"
             "X-Frame-Options: SAMEORIGIN\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n\r\n",
             responseCode, contentType, contentLength);

    if (write(retClient, buff, strlen(buff)) < 0)
    {
        perror("Error writing headers");
    }
}

void httpResponse(int retClient, char *responseMsg)
{
    if (write(retClient, responseMsg, strlen(responseMsg)) < 0)
    {
        perror("Error writing body");
    }
}

int sendFile(int retClient, char *contentType, fileType *file)
{
    if (NULL == file)
    {
        return 0;
    }
    httpHeaders(retClient, 200, contentType, file->size);
    if (write(retClient, file->fileContent, file->size) < 0)
    {
        error = "sendFile() error";
        return 0;
    }

    return 1;
}

httpRequestType *parseHttp(char *str)
{
    httpRequestType *request;
    char *p;
    request = malloc(sizeof(httpRequestType));
    memset(request, 0, sizeof(httpRequestType));

    for (p = str; *p && *p != ' '; p++)
    {
        // no implementation.
    }

    if (*p == ' ')
    {
        *p = 0;
    }
    else
    {
        error = "parseHttp() NOSPACE error";
        free(request);
        return 0;
    }
    strncpy(request->method, str, REQ_METODE_SIZE - 1);

    for (str = ++p; *p && *p != ' '; p++)
    {
        // no implementation.
    }

    if (*p == ' ')
    {
        *p = 0;
    }
    else
    {
        error = "parseHttp() 2ND NOSPACE error";
        free(request);
        return 0;
    }
    strncpy(request->url, str, REQ_URL_SIZE - 1);
    return request;
}

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
    server.sin_family = AF_INET;
    server.sin_port = htons(portNumber);

    if (bind(socketFd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        error = "bind() eroor";
        close(socketFd);
        return 0;
    }

    if (listen(socketFd, 5) < 0)
    {
        error = "listen() error";
        close(socketFd);
        return 0;
    }

    return socketFd;
}

int clientAccept(int retServer)
{
    int retClient;
    socklen_t addrLen;
    struct sockaddr_in client;

    memset(&client, 0, sizeof(client));
    retClient = accept(retServer, (struct sockaddr *)&client, &addrLen);
    if (retClient < -1)
    {
        error = "accept() error";
        return 0;
    }

    return retClient;
}

char *clientRead(int retClient)
{
    static char buff[REQ_BUFFER_SIZE];
    memset(buff, 0, REQ_BUFFER_SIZE);

    if (read(retClient, buff, REQ_BUFFER_SIZE - 1) < 0)
    {
        error = "read() error";
        return 0;
    }

    return buff;
}
void clientConnection(int retServer, int retClient)
{
    httpRequestType *requestRet;
    char buff[REQ_BUFFER_SIZE];
    char *recvBuffer;
    char *responseMsg;

    recvBuffer = clientRead(retClient);
    if (!recvBuffer)
    {
        fprintf(stderr, "%s\n", error);
        close(retClient);
        return;
    }
    requestRet = parseHttp(recvBuffer);
    if (!requestRet)
    {
        fprintf(stderr, "%s\n", error);
        close(retServer);
        return;
    }
    if (strcmp(requestRet->method, "GET") == 0 && strncmp(requestRet->url, "/img/", 5) == 0)
    {
        if (strstr(requestRet->url, ".."))
        {
            httpHeaders(retClient, 403, "text/plain", 0);
            httpResponse(retClient, "Forbidden");
            close(retClient);
            return;
        }

        char filePath[256];
        snprintf(filePath, sizeof(filePath), "%s/%s", BASE_DIR, requestRet->url + 5);

        const char *contentType = getContentType(filePath);
        fileType file;
        file.fileContent = loadFileContent(filePath, &file.size);

        if (NULL == file.fileContent)
        {
            httpHeaders(retClient, 404, "text/plain", strlen(error));
            httpResponse(retClient, error);
        }
        else
        {
            sendFile(retClient, contentType, &file);
            free(file.fileContent);
        }
    }

    if (strcmp(requestRet->method, "GET") == 0 && strcmp(requestRet->url, "/app/webpages") == 0)
    {
        const char *responseMsg =
            "<html style=\"background-color: #f4f4f9; color: #333; font-family: Arial, sans-serif; text-align: center; padding: 20px;\">"
            "<h1 style=\"color: #007BFF;\">Hello World</h1>"
            "<img src='/img/cProgramming.jpg' alt='image'/>"
            "<p style=\"font-size: 16px;\">This is a styled HTTP response message.</p>"
            "</html>";
        httpHeaders(retClient, 200, "text/html", strlen(responseMsg));
        httpResponse(retClient, responseMsg);
    }
    else
    {
        responseMsg = "File not Found";
        httpHeaders(retClient, 400, "text/plain", strlen(responseMsg));
        httpResponse(retClient, responseMsg);
    }

    free(requestRet);
    close(retClient);
    return;
}

int main(int argc, char *argv[])
{
    int retServer, retClient;
    char *port;

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

    printf("Listening on %s:%s\n", LISTENADDR, port);

    while (1)
    {
        retClient = clientAccept(retServer);
        if (!retClient)
        {
            fprintf(stderr, "%s\n", error);
            continue;
        }
        printf("Incoming connection\n");

        pid_t pid = fork();
        if (pid == 0)
        {
            close(retServer);
            clientConnection(retServer, retClient);
            close(retClient);
            exit(0);
        }
        else if (pid > 0)
        {
            close(retClient);
        }
        else
        {
            perror("fork error");
        }
    }

    return 0;
}