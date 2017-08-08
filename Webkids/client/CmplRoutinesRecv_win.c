#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(char *message);

WSABUF dataBuf;
char buf[BUF_SIZE];
int recvBytes = 0;

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET hListenSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;

	WSAOVERLAPPED overlapped;
	WSAEVENT evObj;

	int recvAdrSz, idx, flags = 0;
	if (argc != 2) {
		printf("Usage: : %s <PORT>\n", argv[0]);
		edit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup error!");
	}

	hListenSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hListenSock, (SOCKADDR *)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}
	if (listen(hListenSock, 5) == SOCKET_ERROR) {
		ErrorHandling("listen() error");
	}

	recvAdrSz = sizeof(recvAdr);
	hRecvSock = accept(hListenSock, (SOCKADDR *)&recvAdr, &recvAdrSz);
	if (hRecvSock == INVALID_SOCKET) {
		ErrorHandling("accept() error");
	}

	memset(&overlapped, 0, sizeof(overlapped));
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;
	evObj = WSACreateEvent();		//	Dummy event object

	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompRoutine) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSA_IO_PENDING) {
			puts("Background data receiving");
		}
	}

	idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
	if (idx == WAIT_IO_COMPLETION) {
		puts("Overlapped I/O Completed");
	}
	else {
		ErrorHandling("WSARecv() error");
	}
	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hListenSock);
	WSACleanup();
	return 0;
}

void CALLBACK CompRoutine(DWORD dwError, DWORD recvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags) {
	if (dwError != 0){
		ErrorHandling("CompRoutine error");
	}
	else {
		recvBytes = recvBytes;
		printf("Received message: %s\n", buf);
	}
}

void ErrorHandling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}