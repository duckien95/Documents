// TCPCLient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<winsock2.h>
#include<stdio.h>
#include<conio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = 0;
	char host[256];
	ret = WSAStartup(wVersion, &wsaData);
	SOCKET client;
	SOCKADDR_IN serverAddr;
	printf("Nhap dia chi server:");
	scanf("%s", host);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(host);
	serverAddr.sin_port = htons(8888);

	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ret = connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret = SOCKET_ERROR)
	{
		printf("loi %d", WSAGetLastError());
		goto END;
	};
	char buf[1024];
	int len = 0;
	while(1)
	{
		gets(buf);
		len = send(client, buf, strlen(buf), 0);
		if(len == SOCKET_ERROR) break;
	}
END:
	getch();
	closesocket(client);
	WSACleanup();

}

