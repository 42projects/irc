#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client
{
public:
	Client(int clientFd);
	~Client();

private:
	int _clientFd;
	unsigned char _regist;
};

#endif