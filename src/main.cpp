#include <iostream>
#include <cstdlib>
#include "Server.hpp"
#include <signal.h>

Server *server;

int check_port(char *port)
{
	int i = 0;
	while(port[i])
	{
		if (port[i] < '0' || '9' < port[i])
		{
			std::cout << "Port must be number\n";
			return (-1);
		}
		i++;
	}
	return (0);
}

void	sigIntHandler(int num)
{
	if (num == SIGINT)
		delete server;
	exit(1);
}

void	sigQuitHandler(int num)
{
	if (num == SIGQUIT)
		delete server;
	exit(1);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./server <port> <password>\n";
		return 1;
	}
	if (check_port(argv[1]) == -1)
		return 1;
	signal(SIGINT, sigIntHandler);
	signal(SIGQUIT, sigQuitHandler);
	if (atoi(argv[1]) < 0 || atoi(argv[1]) > 65535)
	{
		std::cout << "Port Range Error\n";
		return 1;
	}
	server = new Server(atoi(argv[1]), argv[2]);
	if (server->execute() < 0)
	{
		delete server;
		return 1;
	}
	delete server;
	return 0;
}