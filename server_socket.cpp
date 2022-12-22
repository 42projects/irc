#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>


#define BUFFER_SIZE 5

void error_handling(char *message);
int main(int argc, char **argv)
{
	int serv_sock, clnt_sock;
	char message[BUFFER_SIZE];
	int str_len;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if (argc!=2)
	{
		std::cout << "argument error" << std::endl;
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
	{
		error_handling("error");
	}
	memset(&serv_addr, 0 , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("error");
	}

	if (listen(serv_sock, 5) == -1)
	{
		error_handling("error");
	}
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
	{
		error_handling("error");
	}

	sleep(5);
	while(1)
	{
		sleep(1);
		str_len = read(clnt_sock, message, BUFFER_SIZE);
		if (str_len == 0)
		{
			break;
		}
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}
	close(clnt_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}