#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma warning(disable:4996)
//inet_addr() �Լ� ��� �Ұ�. InetPton�� ����ؾ� �ϳ� ��� ����� �� ���� �ӽ÷� ����.
void ErrorHandling(char *message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	//socket �Լ��� ��ȣ���� ������ ���� ����
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen = 0;
	int idx = 0, readLen = 0;
	if (argc != 3) {
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	//socket �Լ�ȣ���� ���ؼ� TCP ������ ����
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKET_ADDRESS*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	while (readLen = recv(hSocket, &message[idx++], 1, 0)) {
		//recv �Լ� ȣ���� ���� ���ŵ� �����͸� 1����Ʈ�� �а� �ִ�.
		if (strLen == -1) {
			ErrorHandling("read() error!");
		}
		strLen += readLen;
		//41�࿡�� 1����Ʈ�� �а� �ֱ� ������ strLen�� ���� ������ 1�� �����ϰ� �ִ�.
	}
	
	printf("Mdssage from server : %s\n", message);
	printf("Function read call count: %d\n", strLen);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputs('\n', stderr);
	exit(1);
}