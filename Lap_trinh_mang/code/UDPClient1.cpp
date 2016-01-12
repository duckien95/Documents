// UDPClient1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = 0;
	SOCKET sender;
	SOCKADDR_IN receiverAddr;
	char buf[1024];
	int len;

	ret = WSAStartup(wVersion,&wsaData);

	// create a socket
	sender = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (sender == SOCKET_ERROR)
	{
		printf("Error: %d\n",WSAGetLastError());
		getchar();
		WSACleanup();
		return 0;
	}

	// fill the destination address
	receiverAddr.sin_family = AF_INET;
	receiverAddr.sin_port = htons(8888);
	receiverAddr.sin_addr.s_addr = inet_addr("10.2.0.1");

	while (1)
	{
		printf("Client: ");
		fflush(stdin);
		gets(buf);
		len = sendto(sender,buf,strlen(buf),0,(SOCKADDR*)&receiverAddr,sizeof(receiverAddr));
		if (strcmp(buf,".") == 0) break;
	}

	closesocket(sender);
	WSACleanup();
	return 0;
}

