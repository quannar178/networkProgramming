// UDPClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>



int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    char* buffer = (char*)calloc(1024, 1);
    while (strcmp(buffer, "quit") != 0)
    {
        fflush(stdin);
        gets_s(buffer, 1024);
        sendto(s, buffer, strlen(buffer), 0, (sockaddr*)&saddr, sizeof(saddr));
    }

    closesocket(s);
    WSACleanup();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
