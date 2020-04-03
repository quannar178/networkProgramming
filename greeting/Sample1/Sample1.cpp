#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

int main()
{
	WSAData wsaData;
	WORD wVersion;

	wVersion = MAKEWORD(2, 2);

	int r = WSAStartup(wVersion, &wsaData);
	if (r != 0) {
		printf("Version is not support\n");
	}
	else
	{
		printf("Startup successfully\n");
	}


	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET)
	{
		printf("Khoi tao khong thanh cong\n");
	}
	else
	{
		struct addrinfo* result = NULL;
		struct addrinfo* tmp = NULL;
		r = getaddrinfo("216.58.200.14", "http", NULL, &result);

		printf("Gia tri tra ve cua r la: %d\n", r);
		tmp = result;
		while (result != NULL)
		{
			
			if (result->ai_addrlen == sizeof(sockaddr_in))
			{
				struct sockaddr_in addr;
				memcpy(&addr, result->ai_addr, sizeof(addr));
				printf("%s", inet_ntoa(addr.sin_addr));
				addr.sin_family = AF_INET;
				addr.sin_port = htons(80);

				r = connect(s, (sockaddr*)&addr, sizeof(addr));
				if (r == 0)
				{
					break;
				}
				else
				{
					printf("Error!!!");
				}
				
			}
			result = result->ai_next;
			
		}
		freeaddrinfo(tmp);
	}


	WSACleanup();

	return 0;
}
