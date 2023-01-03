#include "Channel.hpp"
#include <iostream>

Channel::Channel(std::string channelName, int fd) : _operator(fd), _channelName(channelName){};

Channel::~Channel(){};

int Channel::getChannelOperator()
{
	return _operator;
}

void Channel::setChannelOperator(int fd)
{
	_operator = fd;
	return ;
}

std::string Channel::getChannelName() { return (_channelName); };

std::vector<int> Channel::getClientFdList()
{
	return _channelClientFdList;
}

void Channel::addClientList(int fd)
{
	_channelClientFdList.push_back(fd);
}

void Channel::removeClientList(int fd)
{
	std::vector<int>::iterator it = findClientIt(fd);
	if (it != _channelClientFdList.end())
		_channelClientFdList.erase(it);
}

std::vector<int>::iterator Channel::findClientIt(int fd)
{
	std::vector<int>::iterator it = _channelClientFdList.begin();
	while (it != _channelClientFdList.end())
	{
		if ((*it) == fd)
			return (it);
		it++;
	}
	return (_channelClientFdList.end());
}

bool Channel::checkClientInChannel(int fd)
{
	std::vector<int>::iterator it = findClientIt(fd);
	if (it != _channelClientFdList.end())
		return true;
	return false;
}