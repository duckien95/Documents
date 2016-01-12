#include "stdafx.h"
#include <winsock2.h>
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	int ret = WSAStartup(wVersion, &wsaData);
	SOCKET receiver= socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
	SOCKADDR_IN receiverAddr, senderAddr;
	int senderAddrLen = sizeof(senderAddr);
	receiverAddr.sin_family= AF_INET;
	receiverAddr.sin_port = htons(8888);
	receiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);


	ret = bind(receiver,(sockaddr*)&receiverAddr, sizeof(receiverAddr) );
	if(ret == SOCKET_ERROR)
	{
		closesocket(receiver);
		WSACleanup();
		return 0;
	};

	char buf[1024];
	int len;
	while(1)
	{
		len = recvfrom(receiver, buf, 1024, 0, (sockaddr*)&senderAddr, &senderAddrLen);
		buf[len] = 0;
		printf("[%s:%d]%s\n", inet_ntoa(senderAddr.sin_addr), htons(senderAddr.sin_port),buf);
	}
}  