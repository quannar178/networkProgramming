#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int c = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in saddr;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int r = connect(c, (sockaddr*)&saddr, sizeof(saddr));

    char* buffer = (char*)calloc(1024, 1);
    while (strcmp(buffer, "quit") != 0) {
        fflush(stdin);
        memset(buffer, 0, 1024);
        gets(buffer);
        send(c, buffer, strlen(buffer), 0);
        memset(buffer, 0, 1024);
        recv(c, buffer, 1024, 0);
        printf("%s \n", buffer);
    }

    free(buffer);
    buffer = NULL;
    close(c);


    return 0;
}