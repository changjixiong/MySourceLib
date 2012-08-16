#include <stdio.h>
#include <stdlib.h>
#include <WINSOCK2.H>

#pragma comment(lib, "ws2_32.lib")
int main()
{
	WORD wVersionRequest;
	WSADATA wsaData;
	int err;

	wVersionRequest=MAKEWORD(1,1);

	err=WSAStartup(wVersionRequest,&wsaData);

	if (err!=0)
	{
		return 0;
	}

	if (HIBYTE(wsaData.wVersion)!=1||
		LOBYTE(wsaData.wVersion)!=1)
	{
		WSACleanup();
		return 0;
	}

	SOCKET sockSer=socket(AF_INET,SOCK_DGRAM,0);

	SOCKADDR_IN addSer;
	SOCKADDR_IN	addClient;

	addSer.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addSer.sin_family=AF_INET;
	addSer.sin_port=htons(6000);

	bind(sockSer,(sockaddr*)&addSer,sizeof(sockaddr));

	int len=sizeof(sockaddr);
	char recvBuff[100];
	memset(recvBuff, 0, 100);

	recvfrom(sockSer,recvBuff,100,0,(sockaddr*)&addClient,&len);

	printf(recvBuff);

	closesocket(sockSer);
	WSACleanup();

	getchar();
	
	return 0;
}
