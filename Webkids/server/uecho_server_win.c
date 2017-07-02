#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET servSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSz;

	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;

	if (argc != 2) {
		printf(":Usage : %s <port>\main", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error");
	}

	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}
	while (1) {
		clntAdrSz = sizeof(clntAddr);
		strLen = recvfrom(servSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSz);
		sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
	}
	closesocket(servSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputs('\n', stderr);
	exit(1);
}