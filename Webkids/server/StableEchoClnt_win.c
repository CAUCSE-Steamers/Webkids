#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#define BUF_SIZE 1024
void ErrorHandling(char *msg);

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;
	char message[BUF_SIZE];
	int strLen, readLen;

	if (argc != 3) {
		printf("Usage: %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		Errorhandling("WSAStartup() error");
	}
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&sendAdr, 0, sizeof(sendAdr));
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_addr.s_addr = inet_addr(argv[1]);
	sendAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR *)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}
	else {
		puts("Connected..........");
	}
	while (1) {
		fputs("Input message(! to quit: ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
			break;
		}
		strLen = strlen(message);
		send(hSocket, message, strLen, 0);
		readLen = 0;
		while (1) {
			readLen += recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);
			if (readLen >= strLen) {
				break;
			}
		}
		message[strLen] = 0;
		printf("Message from serverL %s", message);
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}