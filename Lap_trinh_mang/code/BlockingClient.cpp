// BlockingClient.cpp : 
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>

SOCKET client;
BOOL bIsFinished = false;

DWORD WINAPI ReceiverThread(LPVOID lpParam)
{
	char buf[1024];
	int len = 0;
	while (!bIsFinished)
	{
		len = recv(client,buf,1024,0);
		if (len <= 0) break;
		buf[len] = '\0';
		printf("Server: %s",buf);
		if (buf[len-1] == '\n') buf[len-1] = '\0';
		if (strcmp(buf,".") == 0) break;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = WSAStartup(wVersion,&wsaData);
	char buf[1024];
	int len = 0;
	SOCKADDR_IN serverAddr;

	client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (client == SOCKET_ERROR)
	{
		printf("Error %d while creating socket!\n",WSAGetLastError());
		return 0;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(8888);

	ret = connect(client,(sockaddr*)&serverAddr,sizeof(serverAddr));

	if (ret == SOCKET_ERROR)
	{
	}

	CreateThread(0,0,ReceiverThread,0,0,0);
	while (1)
	{
		fflush(stdin);
		fgets(buf,1024,stdin);
		len = send(client,buf,strlen(buf),0);
		buf[len-1] = '\0';
		if (strcmp(buf,".") == 0) 
		{
			bIsFinished = true;
			break;
		}
	}

	return 0;
}