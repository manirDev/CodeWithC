#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "netinet/in.h"
#include <arpa/inet.h>

#define LISTENADDR "0.0.0.0"
#define REQ_BUFFER_SIZE 0x200
#define REQ_METODE_SIZE 0x008
#define REQ_URL_SIZE    0x080

#define BASE_DIR "./img"

#define RESPONSE_MSG \
"<html style=\"background-color: #f4f4f9; color: #333; font-family: Arial, sans-serif; text-align: center; padding: 20px;\">" \
"<h1 style=\"color: #007BFF;\">Hello World</h1>" \
"<img src=\"/img/cProgramming.jpg\" alt='image'/>"\
"<p style=\"font-size: 16px;\">This is a styled HTTP response message.</p>" \
"</html>"

typedef struct httpRequestType
{
    char method[8];
    char url[128];
}httpRequestType;

typedef struct fileType
{
    char* fileName[64];
    char* fileContent;
    int size;
}fileType;