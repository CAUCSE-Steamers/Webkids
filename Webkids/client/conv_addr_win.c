#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <WinSock2.h>
#pragma warning(disable:4996)
//WSAStringToAddress는 WSAStringToAddressA 또는 WSAStringToAddressW로, 
//WSAAddressToString은 WSAAddressToStringA 혹은 WSAAddressToStringW로 바꿔서 써야 함.
//결과를 돌려보면 깨지는데 왜 깨지는지 모름
int main(int argc, char *argv[]) {
	char *strAddr = "203.211l218.102:9190";

	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	size = sizeof(servAddr);
	WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR *)&servAddr, &size);

	size = sizeof(strAddrBuf);
	WSAAddressToString((SOCKADDR *)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);

	printf("Second conv result: %s \n", strAddrBuf);
	WSACleanup();
	return 0;
}