// BlockingServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

SOCKET clients[64];
SOCKADDR_IN clientAddrs[64];
int length = 0;
BOOL bIsFinished;

DWORD WINAPI SenderThread(LPVOID lpParam)
{
	char buf[1024];
	int len;
	while (1)
	{
		fflush(stdin);
		fgets(buf,1024,stdin);
		for (int i=0;i<64;i++)
		{
			if (clients[i] != -1)
				send(clients[i],buf,strlen(buf),0);
		}
	}
	return 0;
}
DWORD WINAPI ReceiverThread(LPVOID lpParam)
{
	char buf[1024];
	int len = 0;
	int i = (int)lpParam;
	while (!bIsFinished)
	{
		len = recv(clients[i],buf,1024,0);
		if (len <= 0) 
		{
			clients[i] = -1;
			break;
		}
		buf[len] = '\0';
		printf("From %s %d: %s",inet_ntoa(clientAddrs[i].sin_addr),ntohs(clientAddrs[i].sin_port),buf);
		if (buf[len-1] == '\n') buf[len-1] = '\0';
		if (strcmp(buf,".") == 0) 
		{
			clients[i] = -1;
			break;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int i, len;
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = WSAStartup(wVersion,&wsaData);
	SOCKET server;
	SOCKADDR_IN serverAddr;

	server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(8888);

	ret = bind(server,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("Error %d while binding!\n",WSAGetLastError());
		return 0;
	}
	ret = listen(server,10);

	for (i=0; i<64;i++) clients[i] = -1;

	while(1)
	{
		for (i=0;i<64;i++) 
			if (clients[i] == -1) break;
		if (i == 64) 
		{
			printf("Too many connections\n");
			sleep(1000);
			continue;
		}
		len = sizeof(SOCKADDR_IN);
		clients[i] = accept(server,(sockaddr*)&clientAddrs[i],&len);
		CreateThread(0,0,ReceiverThread,(LPVOID)i,0,0);
		printf("There is a connection from %s %d\n",inet_ntoa(clientAddrs[i].sin_addr),ntohs(clientAddrs[i].sin_port));
		length++;
		if (length == 1)
			CreateThread(0,0,SenderThread,0,0,0);
	}

	return 0;
}

