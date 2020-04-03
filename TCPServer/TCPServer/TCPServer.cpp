#include <stdio.h>
#include <WinSock2.h>

void sendTCP(SOCKET c, char* buffer, int len);
int recvTCP(SOCKET c, char* buffer, int len, int count);

int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	saddr.sin_addr.S_un.S_addr = htons(INADDR_ANY);
	bind(s, (sockaddr*)&saddr, sizeof(saddr));
	listen(s, 10);

	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	SOCKET c = accept(s, (sockaddr*)&caddr, &len);
	char* buffer  = (char*) calloc(1024, 1);
	while (0 == 0)
	{
		int r = recvTCP(c, buffer, 1024, 32);
		sendTCP(c, buffer, r);
	}

	return 0;
}

int recvTCP(SOCKET c, char* buffer, int len, int count) // len = nhan duoc toi da, count : muon nhan, nhan xong moi gui di
{
	int r = recv(c, buffer, len, 0);
	printf("%d byte receive\n", r);
	/*while (r < count && r < len)
	{
		r += recv(c, buffer + r, len, 0);
		printf("%d byte receive\n", r);
	}
	return r;*/
}

void sendTCP(SOCKET c, char* buffer, int len)
{
	int s = send(c, buffer, len, 0);
	while (s < len)
	{
		s += send(c, buffer + s, len - s, 0);
	}
}