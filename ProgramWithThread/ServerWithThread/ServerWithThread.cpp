#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>


DWORD WINAPI ClientThread(LPVOID param) {


	char* welcome = (char*)calloc(1024, 1) ;
	memset(welcome, 0, sizeof(welcome));
	strcpy(welcome, "Welcome to Telnet server\n");
	SOCKET c = (SOCKET)param;
	send(c, welcome, strlen(welcome), 0);
	char buffer[1024];
	char cmd[1024];


		//Please enter your username
		memset(welcome, 0, sizeof(welcome));
		strcpy(welcome, "\nEnter username: ");
		send(c, welcome, strlen(welcome), 0);
		memset(buffer, 0, sizeof(buffer));
		recv(c, buffer, sizeof(buffer), 0);
		//Please enter your password
		memset(cmd, 0, sizeof(cmd));
		memset(welcome, 0, sizeof(welcome));
		strcpy(welcome, "\nEnter password: ");
		send(c, welcome, strlen(welcome), 0);
		recv(c, cmd, sizeof(cmd), 0);

		while (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r') {
			buffer[strlen(buffer) - 1] = 0;
		}
		while (cmd[strlen(cmd) - 1] == '\n' || cmd[strlen(cmd) - 1] == '\r') {
			cmd[strlen(cmd) - 1] = 0;
		}

		//Check user and pwd
		FILE* pf = fopen("user.txt", "rt");

		if (pf == NULL) {
			printf("File not found!\n");
			return -1;
		}
		char user[1024], pwd[1024];

		bool check = 0;
		while (!feof(pf))
		{
			memset(user, 0, sizeof(user));
			memset(pwd, 0, sizeof(pwd));
			fscanf(pf, "%s", user);
			fscanf(pf, "%s", pwd);
			if (!strcmp(buffer, user) != 0 && !strcmp(cmd, pwd)) {
				check = true;
				break;
			}
		}

		if (check == false) {
			welcome = (char*)"\nInvalid username or password!\n";
			send(c, welcome, strlen(welcome), 0);
			closesocket(c);
			return -1;
		}
		else {
			welcome = (char*)"\nSuccess\n-----START TELNET------\n";
			send(c, welcome, strlen(welcome), 0);
		}

		fclose(pf);

	while (0 == 0)
	{
		memset(buffer, 0, sizeof(buffer));
		recv(c, buffer, sizeof(buffer), 0);
		while (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r') {
			buffer[strlen(buffer) - 1] = 0;
		}
		printf("SOCKET %d: %s\n", c, buffer);
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "%s > out.txt", buffer);
		system(cmd);
		FILE* f = fopen("out.txt", "rt");
		while (!feof(f)) {
			memset(buffer, 0, sizeof(buffer));
			fgets(buffer, sizeof(buffer), f);
			send(c, buffer, strlen(buffer), 0);
		}
		fclose(f);

	}
	return 0;
}

int main(int argv, char* args[]) {
	

	WSADATA DATA;

	WSAStartup(MAKEWORD(2, 2), &DATA);

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr, caddr;

	int clen = sizeof(caddr);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(args[2]));
	saddr.sin_addr.S_un.S_addr = 0;

	bind(s, (sockaddr*)&saddr, sizeof(saddr));

	listen(s, 10);

	while (0 == 0) {
		DWORD ID = 0;
		SOCKET c = accept(s, (sockaddr*)&caddr, &clen);
		CreateThread(NULL, 0, ClientThread, (LPVOID)c, 0, &ID);
	}
	return 0;
}
