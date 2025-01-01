#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 0x200

char *trl(int timeout)
{
    static char buffer[BUFFER_SIZE];
    fd_set rfds;
    struct timeval tv;
    int ret;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    ret = select(1, &rfds, 0, 0, &tv);
    if (ret && FD_ISSET(0, &rfds))
    {
        memset(buffer, 0, BUFFER_SIZE);
        ret = read(0, buffer, BUFFER_SIZE - 1);
        if (ret < 1)
        {
            return 0;
        }
        ret--;
        buffer[ret] = 0;
        return buffer;
    }
    else
    {
        return 0;
    }
}

int main()
{
    char *name;
    printf("What is your name ? Think fast!\n");
    name = trl(3);
    if (name)
    {
        printf("Hello: %s\n", name);
    }
    else
    {
        printf("To slow!\n");
    }

    return 0;
}