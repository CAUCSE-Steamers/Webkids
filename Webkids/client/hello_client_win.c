#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma warning(disable:4996)
//inet_addr() �Լ� ��� �Ұ�. InetPton�� ����ؾ� �ϳ� ��� ����� �� ���� �ӽ÷� ����.
void ErrorHandling(char *message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	int strLen;
	if (argc != 3) {
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		//���� ���̺귯�� �ʱ�ȭ
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
		//���� ����
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKET_ADDRESS*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		//������ ������ �������� ������ �����û
		ErrorHandling("connect() error!");
	}

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	//�����κ��� ������ ����. recv �Լ��� �ڿ��� ����
	if (strLen == -1) {
		ErrorHandling("read() error!");
	}
	printf("Mdssage from server : %s\n", message);
	closesocket(hSocket);
	WSACleanup();
	//18�࿡�� �ʱ�ȭ�� ���̺귯�� ����
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputs('\n', stderr);
	exit(1);
}