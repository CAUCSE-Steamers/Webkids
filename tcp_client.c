#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len = 0;
	int idx = 0, read_len = 0;

	if(argc != 3)
	{
		printf("Ussage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	//It make TCP socket.
	//If PF_INET and SOCK_STREAM is passed to first and second parameter, 
	//then we can skip IPPROTO_TCP as a third parameter
	if(sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		//It request connection to server program by calling "connect" function
	{
		error_handling("connect() error!");
	}

	while(read_len = read(sock, &message[idx++], 1)){
		//read function called repeatedly. and it read 1 byte per called
		//and it break when read result 0
		if(read_len == -1)
		{
			error_handling("read() error!");
		}
		str_len += read_len;
		//read_len is always 1
		//because read function always read 1 byte.
		//as a result, str_len will be size of read function reads.
	}

	printf("Message from server : %s\n", message);
	printf("Function read call count : %d\n", str_len);
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}