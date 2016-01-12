// TCPCLient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<winsock2.h>
#include<stdio.h>
#include<conio.h>
#include<ws2tcpip.h>

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = 0;
	char host[256];
	ret = WSAStartup(wVersion, &wsaData);
	SOCKET client;
	SOCKADDR_IN serverAddr; //Luu dia chi cua server can noi den
	printf("Nhap dia chi server:");
	scanf("%s", host);
	addrinfo aiHints;
	memset(&aiHints, 0, sizeof(aiHints));
	aiHints.ai_family = AF_INET;
	aiHints.ai_socktype = SOCK_STREAM;
	aiHints.ai_protocol = IPPROTO_TCP;

	/*
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(8888);
	*/

	addrinfo *result;
	ret = getaddrinfo(host, "8888", &aiHints, &result);
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//ret = connect(client,(SOCKADDR *) &serverAddr,sizeof(serverAddr));
	if(ret = SOCKET_ERROR)
	{
		printf("loi %d", ret);
		WSACleanup();
		return 0;
	};
	
	memcpy(&serverAddr, result->ai_addr, result->ai_addrlen);
	printf("Client connecting to %s", host);
	freeaddrinfo(result);
	char buf[1024];
	int len = 0;
	while(1)
	{
		gets(buf);
		len = send(client, buf, strlen(buf), 0);
		if(len == SOCKET_ERROR) break;
	}
	getch();
}

