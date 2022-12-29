#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include <string>
#include "Client.hpp"

class Client;

class Channel{

private :
	int _operator;
	std::string _channelName;
	std::vector<int> _channelClientFdList;

public :
	Channel(std::string channelName, int fd);
	~Channel();
	int getChannelOperator();
	void setChannelOperator(int fd);
	std::string getChannelName();
	std::vector<int> getClientFdList();
	void addClientList(int fd);
	void removeClientList(int fd);
	std::vector<int>::iterator findClientIt(int fd);
	bool checkClientInChannel(int fd);
};

#endif