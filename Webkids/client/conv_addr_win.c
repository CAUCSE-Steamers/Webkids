#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <WinSock2.h>
#pragma warning(disable:4996)
//WSAStringToAddress�� WSAStringToAddressA �Ǵ� WSAStringToAddressW��, 
//WSAAddressToString�� WSAAddressToStringA Ȥ�� WSAAddressToStringW�� �ٲ㼭 ��� ��.
//����� �������� �����µ� �� �������� ��
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