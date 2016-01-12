// TCPClient2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	WSAData wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = 0;
	SOCKET client;
	SOCKADDR_IN serverAddr;
	char buf[1024];
	int len;
	
	ret = WSAStartup(wVersion,&wsaData);
	if (ret == SOCKET_ERROR)
	{
		printf("Error: %d, when WSAStartup, exit now!\n",WSAGetLastError());
		getchar();
		exit(1);
	}

	if (argc != 3)
	{
		printf("Too low or too much parameters! Please try again!\n");
		exit(1);
	}
		
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));
	
	client = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	ret = connect(client,(sockaddr*)&serverAddr, sizeof(serverAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("Error: %d, when connect, exit now!\n",WSAGetLastError());
		goto END;
	}
	while (1)
	{
		printf("Client: ");
		fflush(stdin);
		gets(buf);
		
		len = send(client,buf,strlen(buf),0);
		if (len == SOCKET_ERROR)
		{
			printf("Got an error, break!\n");
			break;
		}

		if(buf[len-1] == '\n') buf[len-1] = '\0';
		if (strcmp(buf,".") == 0) 
		{
			printf("End of transmission, exit now!\n");
			break;
		}
	}
END:
	closesocket(client);
	WSACleanup();
	return 0;
}

