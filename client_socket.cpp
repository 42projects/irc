#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error_handling(char *message);

int main(int argc, char **argv)
{
	int sock;
	char message[BUFFER_SIZE];
	int str_len;

	char MSG1[] = "Good";
	char MSG2[] = "Evening";
	char MSG3[] = "Everybody!";

	struct sockaddr_in serv_addr;

	if (argc!=4)
	{
		std::cout << "usage: ./<client> <ip address> <port> <message>" << std::endl;
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		error_handling("socket error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect error");
	}
	write(sock, argv[3], strlen(argv[3]));

	str_len = read(sock, message, BUFFER_SIZE);
	message[str_len] = 0;
	std::cout << "서버로부터 전송된 메시지: " << message << std::endl;

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}