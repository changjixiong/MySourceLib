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

	SOCKET sockClient=socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addr;

	addr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	addr.sin_family=AF_INET;
	addr.sin_port=htons(6000);

	sendto(sockClient,"test",strlen("test")+1,0,(sockaddr*)&addr,sizeof(sockaddr));

	closesocket(sockClient);
	WSACleanup();

	getchar();
	return 0;
}
