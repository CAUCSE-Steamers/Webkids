#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma warning(disable:4996)
//inet_addr() 함수 사용 불가. InetPton을 사용해야 하나 사용 방법을 잘 몰라 임시로 지정.
void ErrorHandling(char *message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	//socket 함수의 반호나값 저장을 위해 선언
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
	//socket 함수호출을 통해서 TCP 소켓을 생성
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
		//recv 함수 호출을 통해 수신된 데이터를 1바이트씩 읽고 있다.
		if (strLen == -1) {
			ErrorHandling("read() error!");
		}
		strLen += readLen;
		//41행에서 1바이트씩 읽고 있기 때문에 strLen의 값은 실제로 1씩 증가하고 있다.
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