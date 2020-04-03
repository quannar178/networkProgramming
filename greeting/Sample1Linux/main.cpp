#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <malloc.h>
#include <string.h>

int main()
{
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct addrinfo* result = NULL;

    getaddrinfo("vnexpess.net", "http", NULL, &result);

    struct addrinfo* tmp = result;

    while (result != NULL)
    {
        struct sockaddr_in addr;
        if (result->ai_addrlen == sizeof(addr))
        {
            memcpy(&addr, result->ai_addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(80);
            int r = connect(s, (sockaddr* )&addr, sizeof(addr));
            if (r == 0)
            {
                break;
            }
            else
            {
                printf("Loi");
            }
        }
        result = result->ai_next;
    }

    freeaddrinfo(tmp);
    return 0;
}