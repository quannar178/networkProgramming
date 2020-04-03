#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {

	if (argv[2] == NULL || argv[1] == NULL|| argv[0] == NULL) {
		printf("Tham so dien vao chua du\n");
		return 0;
	}

	//Kiểm tra tham số dòng lệnh đầu tiên.
	if (strcmp(argv[0], "TCPClient") != 0) {
		printf("Tham so dau tien sai\n");
		return 0;
	}


	//khoi tao winsock 2.2
	WSADATA data;
	int r = WSAStartup(MAKEWORD(2, 2), &data);

	if (r != 0) {
		printf("Phien ban khong duoc ho tro.\n");
		return 0;
	}

	SOCKET c = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (c == INVALID_SOCKET) {
		printf("Khong tao duoc socket. \n");
		WSACleanup();
	}

	//Kiểm tra địa chỉ hoặc IP đã nhập - IPv4

	struct sockaddr_in caddr;
	caddr.sin_family = AF_INET;

	USHORT port = (USHORT) atoi(argv[2]);

	if (port == 0) {
		printf("Tham so Port khong ton tai");
		WSACleanup();
		return 0;
	}
	else
	{
		caddr.sin_port = port;
		printf("Gia tri port nhan vao la: %d\n", port);
	}


	struct addrinfo* result = NULL;
	struct addrinfo* tmp = NULL;
	

	r = getaddrinfo(argv[1], "http", NULL, &result);

	tmp = result;

	//Khi tồn tại IP ứng với địa chỉ đã cho. thì r != 0.
	if (r == 0) {
		while (result != NULL)
		{
			if (result->ai_addrlen == sizeof(caddr)) {
				memcpy(&caddr, result->ai_addr, sizeof(caddr));
				printf("Lay duoc IP: %s \n", inet_ntoa(caddr.sin_addr));
				r = connect(c, (sockaddr*)&caddr, sizeof(caddr));
				if (r == 0)
				{
					break;
				}
				else
				{
					printf("Khong ket noi duoc cong\n");
				}
			}
			result = result->ai_next;
		}
		freeaddrinfo(tmp);
	}
	else
	{
		printf("Dia chi hoac IP da cho khong ton tai!");
		WSACleanup();
		return 0;
	}


	//Trao doi du lieu, khong thoat ra khoi while duoc vi gia tri gui ve khong "quit".
	char* buffer = (char*)calloc(1024, 1);
	while (strcmp(buffer, "quit") != 0) {
		fflush(stdin);
		memset(buffer, 0, 1024);
		fgets(buffer, 1024, stdin);
		send(c, buffer, strlen(buffer), 0);
		memset(buffer, 0, 1024);
		recv(c, buffer, 1024, 0);
		printf("%s \n", buffer);
	}

	free(buffer);
	buffer = NULL;
	closesocket(c);
	WSACleanup();



}

