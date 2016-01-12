// TCPServer2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2,2);
	int ret = 0;
	SOCKET server, client;
	SOCKADDR_IN serverAddr, clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	FILE *fHello, *fText;
	char buf[1024];
	int len;

	ret = WSAStartup(wVersion,&wsaData);

	if (argc != 4)
	{
		printf("Too much or too low parameters!\n");
		exit(1);
	}

	server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(server,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("Error: %d, when bind, close!",WSAGetLastError());
		goto END;
	}

	ret = listen(server,10);
	printf("Listening...\n");
	if (ret == SOCKET_ERROR)
	{
		printf("Error: %d, when listening, close!",WSAGetLastError());
		goto END;
	}

	client = accept(server,(sockaddr*)&clientAddr,&clientAddrLen);
	if (client == SOCKET_ERROR)
	{
		printf("Error while accepting client!\n");
		closesocket(client);
		goto END;
	}
	
	printf("There is a connection from %s %d!\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port));
	fopen_s(&fHello,argv[2],"r");
	if (fHello == NULL)
	{
		printf("Error when opening hello file or file not found. Exit now!");
		closesocket(client);
		goto END;
	}

	fopen_s(&fText,argv[3],"w");
	if (fText == NULL)
	{
		printf("Error when creating text file or file not found. Exit now!\n");
		closesocket(client);
		fclose(fHello);
		fclose(fText);
	}

	fgets(buf,1024,fHello);
	len = send(client,buf,strlen(buf),0);
	while (1)
	{
		len = recv(client,buf,1024,0);
		if (len >= 0)
		{
			buf[len] = '\0';
			printf("Client: %s",buf);
			fprintf(fText,"%s",buf);
			if (buf[len-1] == '\n') buf[len-1] = '\0';
			else
			{
				printf("\n");
				fprintf(fText,"\n");
			}
			if (strcmp(buf,".") == 0)
			{
				send(client,"bye",3,0);
				closesocket(client);
				fclose(fHello);
				fclose(fText);
				break;
			}
		}
	}

END:
	closesocket(server);
	WSACleanup();
	return 0;
}

