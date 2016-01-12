// UDPServer1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	SOCKET receiver;
	SOCKADDR_IN addr, source;
	char buf[1024];
	int len = sizeof(source);
	int ret, datalen;

	ret = WSAStartup(wVersion,&wsaData);

	// Creating a socket
	receiver = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if (receiver == SOCKET_ERROR)
	{
		printf("Error: %d\n",WSAGetLastError());
		getchar();
		WSACleanup();
		return 0;
	}

	// Initialise address and port
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8888);

	// Bind socket
	ret = bind(receiver,(sockaddr*)&addr,sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR)
	{
		printf("Error: %d\n",WSAGetLastError);
		goto END;
	}

	// Waiting for datagram
	while (1)
	{
		// Receiving data
		datalen = recvfrom(receiver,buf,1024,0,(sockaddr*)&source,&len);
		// Checking length
		if (datalen > 0)
		{
			buf[datalen] = '\0';
			printf("Data from %s %d: %s",inet_ntoa(source.sin_addr),htons(source.sin_port),buf);
			if (buf[datalen-1] == '\n') buf[datalen-1] = '\0';
			else printf("\n");
			if (strcmp(buf,".") == 0) break;
		}
	}

END:
	getchar();
	closesocket(receiver);
	WSACleanup();
	return 0;
}

