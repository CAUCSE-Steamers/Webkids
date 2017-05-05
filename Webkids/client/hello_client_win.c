#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma warning(disable:4996)
//inet_addr() 함수 사용 불가. InetPton을 사용해야 하나 사용 방법을 잘 몰라 임시로 지정.
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
		//소켓 라이브러리 초기화
		ErrorHandling("WSAStartup() error!");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
		//소켓 생성
	if (hSocket == INVALID_SOCKET) {
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKET_ADDRESS*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		//생성된 소켓을 바탕으로 서버에 연결요청
		ErrorHandling("connect() error!");
	}

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	//서버로부터 데이터 수신. recv 함수는 뒤에서 설명
	if (strLen == -1) {
		ErrorHandling("read() error!");
	}
	printf("Mdssage from server : %s\n", message);
	closesocket(hSocket);
	WSACleanup();
	//18행에서 초기화한 라이브러리 해제
	return 0;
}

void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputs('\n', stderr);
	exit(1);
}