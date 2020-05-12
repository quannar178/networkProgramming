#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>

char* g_html = NULL;

void ScanFile(SOCKET c, char* path) {
	char header[1024];
	memset(header, 0, 1024);
	strcpy(header, "HTTP/1.1 200 OK\nServer: NBQ\n");
	
	char* patter = strstr(path, "QUAN");
	patter[0] = 0;

	if (_stricmp(path + strlen(path) - 4, ".jpg") == 0 ) {
		sprintf(header + strlen(header), "Content-type: image/jpeg\n");
	}
	else if (_stricmp(path + strlen(path) - 4, ".mp4") == 0) {
		sprintf(header + strlen(header), "Content-type: video/mp4\n");
	}
	else
	{
		sprintf(header + strlen(header), "Content-type: application/octet-stream\n");
	}

	char fullpath[1024];
	memset(fullpath, 0, 1024);
	sprintf(fullpath, "C:%s", path);



	FILE* f = fopen(fullpath, "rb");

	fseek(f, 0, SEEK_END);
	int flen = ftell(f);
	fseek(f, 0, SEEK_SET);
	sprintf(header + strlen(header), "Content-length: %d\n\n", flen);

	send(c, header, strlen(header), 0);

	char buffer[1024];
	while (!feof(f)) {
		int r = fread(buffer, 1, sizeof(buffer), f);
		send(c, buffer, r, 0);
	}
	fclose(f);
}

void ScanFolder(char* path) {
	WIN32_FIND_DATAA DATA;
	char fullpath[1024];
	memset(fullpath, 0, sizeof(fullpath));

	if (path[strlen(path) - 1] == '/')
		sprintf(fullpath, "C:%s*.*", path);
	else
		sprintf(fullpath, "C:%s/*.*", path);

	HANDLE hFind = FindFirstFileA(fullpath, &DATA);
	char tempHtml[1024];
	memset(tempHtml, 0, sizeof(tempHtml));

	if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		if(path[strlen(path) - 1] == '/')
			sprintf(tempHtml, "<a href=\"%s%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);
		else
			sprintf(tempHtml, "<a href=\"%s/%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);

	}
	else
		if (path[strlen(path) - 1] == '/')
			sprintf(tempHtml, "<a href=\"%s%sQUAN\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
		else
			sprintf(tempHtml, "<a href=\"%s/%sQUAN\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);

	int oldLen = g_html != NULL ? strlen(g_html) : 0;
	g_html = (char*)realloc(g_html, oldLen + strlen(tempHtml) + 1);
	sprintf(g_html + oldLen, "%s", tempHtml);

	if (hFind != INVALID_HANDLE_VALUE) {
		while (FindNextFileA(hFind, &DATA) == TRUE)
		{
			memset(tempHtml, 0, sizeof(tempHtml));

			if (DATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if(path[strlen(path) - 1] =='/')
					sprintf(tempHtml, "<a href=\"%s%s\"><b>%s</b></a><br>",path, DATA.cFileName, DATA.cFileName);
				else
					sprintf(tempHtml, "<a href=\"%s/%s\"><b>%s</b></a><br>", path, DATA.cFileName, DATA.cFileName);
			}
			else {
					if (path[strlen(path) - 1] == '/')
						sprintf(tempHtml, "<a href=\"%s%sQUAN\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
					else
						sprintf(tempHtml, "<a href=\"%s/%sQUAN\">%s</a><br>", path, DATA.cFileName, DATA.cFileName);
			}
			int oldLen = g_html != NULL ? strlen(g_html) : 0;
			g_html = (char*)realloc(g_html, oldLen + strlen(tempHtml) + 1);
			sprintf(g_html + oldLen, "%s", tempHtml);
		}


	}
}
DWORD WINAPI ClientThread(LPVOID params) {
	SOCKET c = (SOCKET)params;
	char buffer[1024];
	

	memset(buffer, 0, sizeof(buffer));

	recv(c, buffer, sizeof(buffer), 0);

	g_html = (char*)calloc(1024, 1);
	//sprintf(g_html, "<html><h1>Simple HTTP File Server</h1><br>");

	char ACTION[1024];
	char PATH[1024];
	char VER[2024];

	memset(ACTION, 0, sizeof(ACTION));
	memset(PATH, 0, sizeof(PATH));
	memset(VER, 0, sizeof(VER));

	sscanf(buffer, "%s%s%s", ACTION, PATH, VER);

	//char RealPath[1024];
	//memset(RealPath, 0, sizeof(RealPath));
	//sprintf(RealPath, "C:\\%s\*.*", PATH);


	//
	while (strstr(PATH, "%20") != NULL) {
		char* found = strstr(PATH, "%20");
		found[0] = ' ';
		strcpy(found + 1, found + 3);
	}

	if (strstr(PATH, "QUAN")) {
		ScanFile(c, PATH);
	}
	else
	{
		char* header = (char*)"HTTP/1.1 200 OK\nServer: NBQ\nContent-type: text/html\n\n";
		send(c, header, strlen(header), 0);
		ScanFolder(PATH/*, g_html + strlen(g_html)*/);
		g_html = (char*)realloc(g_html, strlen(g_html) + 8);
		sprintf(g_html + strlen(g_html), "</html>");
		send(c, g_html, strlen(g_html), 0);
	}

	closesocket(c);
	return 0;
}

int main() {
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	saddr.sin_addr.S_un.S_addr = 0;

	bind(s, (sockaddr*)&saddr, sizeof(saddr));
	listen(s, 10);

	while (true)
	{
		DWORD ID = 0; 
		struct sockaddr_in caddr;
		int len = sizeof(caddr);

		SOCKET c = accept(s, (sockaddr*)&caddr, &len);

		CreateThread(NULL, 0, ClientThread, (LPVOID)c, 0, &ID);
	}
	return 0;
}
