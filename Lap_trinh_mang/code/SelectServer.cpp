// SelectServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = WSAStartup(wVersion,&wsaData);
	SOCKET server,clients[64],tmp;
	SOCKADDR_IN serverAddr,clientAddrs[64],tmpAddr;
	int clientAddrLen = sizeof(SOCKADDR_IN);
	int i, count = 0;
	for (i=0;i<64;i++) clients[i]= 0;


	server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (server == SOCKET_ERROR)
	{
		printf("Error: %d\n",WSAGetLastError());
		return 0;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(8888);

	ret = bind(server,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("Error %d while binding!\n",WSAGetLastError());
		closesocket(server);
		return 0;
	}

	ret = listen(server,10);

	fd_set readfds;
	char buf[1024];
	int len;
	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(server,&readfds);
		for (i=0;i<64;i++)
			if (clients[i]>0) FD_SET(clients[i],&readfds);
		ret = select(0,&readfds,0,0,0);
		if (ret == SOCKET_ERROR)
		{
			printf("Error %d!\n", WSAGetLastError());
			break;
		}
		
		if (FD_ISSET(server,&readfds))
		{
			for (i=0;i<64;i++)
				if (clients[i] == 0) break;
			if (i==64)
			{
				printf("Too crowd!\n");
				tmp = accept(server,(sockaddr*)&tmpAddr,&clientAddrLen);
				shutdown(tmp,SD_BOTH);
				closesocket(tmp);
				continue;
			}

			clients[i] = accept(server,(sockaddr*)&clientAddrs[i],&clientAddrLen);
			printf("There is a connection from %s %d\n",inet_ntoa(clientAddrs[i].sin_addr),ntohs(clientAddrs[i].sin_port));
			count++;
		}
		for (i=0;i<64;i++)
			if (clients[i] > 0)
				if (FD_ISSET(clients[i],&readfds))
				{
					len = recv(clients[i],buf,1024,0);
					if (buf[len-1] == '\n') buf[len-1] = '\0';
					else buf[len] = 0;
					if (strcmp(buf,".") == 0)
					{
						printf("Client %s %d stop connecting\n",inet_ntoa(clientAddrs[i].sin_addr),ntohs(clientAddrs[i].sin_port));
						clients[i] = 0;
						count--;
						if (count == 0) exit(0);
						continue;
					}
					else
						printf("Client %s %d: %s\n",inet_ntoa(clientAddrs[i].sin_addr),ntohs(clientAddrs[i].sin_port),buf);
				}
	}
	return 0;
}

