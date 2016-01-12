// TCPServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<winsock2.h>
#include<conio.h>
int main(int argc, char* argv[])
{
	WSADATA WSAData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = 0;
	ret = WSAStartup(wVersion, &WSAData);
	SOCKET server;
	SOCKADDR_IN serverAddr;

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//IPAddress serverip = IPAddress.parse("10.2.2.1");
	serverAddr.sin_family = AF_INET;
	for (int i=1;i<argc;i++)
	{
		printf("%s",argv[i]);
	}
	
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = htonl("10.2.2.1");
	ret = bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret == SOCKET_ERROR)
	{
		printf("Loi : %d", WSAGetLastError());
	} 
	else
	{
		listen(server, 10);
		printf("Dang doi ket noi tu client ...\n");
		SOCKET client;
		SOCKADDR_IN clientAddr;
		int clientAddrLen = sizeof(clientAddr);
		client = accept(server, (sockaddr*)&clientAddr, clientAddrLen );
		if(client != SOCKET_ERROR)
		{
			printf("Co ket noi tu : %s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			char buf[1024];
			int len = 0;
			while(1)
			{
				len = recv(client, buf, 1024, 0);
				if(len >= 0 )
				{
					buf[len] = 0;
					printf("client : %s", buf);
					if(strcmp(buf,"bye")==0)
					{
						send(client, "bye!",4, 0);
						closesocket(client);
						break;
					}
				}
				else
				{
					printf("Co loi !!");
					break;
				};
				gets(buf);
				len = send(client, buf, strlen(buf), 0);
			}
		}
	}
}

