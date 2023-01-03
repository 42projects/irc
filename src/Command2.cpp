#include "Command.hpp"
#include "Server.hpp"

void Command::privmsg(std::vector<std::string> s, Client *client)
{
	if (s.size() <= 2)
	{
		makeNumericReply(client->getClientFd(), ERR_NEEDMOREPARAMS, "PRIVMSG :Not enough parameters");
		return ;
	}
	std::vector<std::string> receiver = ft_split(s[1], ",");
	std::vector<std::string>::iterator receiverIt = receiver.begin();
	while (receiverIt != receiver.end())
	{
		if ((*receiverIt)[0] == '#')
		{
			if (_server->findChannel(*receiverIt) == NULL)
				makeNumericReply(client->getClientFd(), ERR_NOSUCHCHANNEL, *receiverIt + " :No such channel");
			else
			{
				if (client->findMyChannelIt(*receiverIt) != client->getMyChannelList().end())
					channelMessage(string_join(2, s), client, _server->findChannel(*receiverIt));
				else 
					makeNumericReply(client->getClientFd(), ERR_NOSUCHCHANNEL, *receiverIt + " :Client doesn't belong to channel");
			}
		}
		else
		{
			if (_server->findClient(*receiverIt) == NULL)
				makeNumericReply(client->getClientFd(), ERR_NOSUCHNICK, *receiverIt + " :No such nick");
			else
			{
				Client *tmpClient = _server->findClient(*receiverIt);
				if (tmpClient != NULL)
					makePrivMessage(tmpClient, client->getNickName(), tmpClient->getNickName(), string_join(2, s) + "\r\n");
					// tmpClient || _server->findClient(*receiverIt);
			}
		}
		receiverIt++;
	}
}

void Command::notice(std::vector<std::string> s, Client *client)
{
	Client *receiver = _server->findClient(s[1]);
	if (receiver != NULL)
		receiver->appendMsgBuffer(makeFullname(client->getClientFd()) + " NOTICE " + receiver->getNickName() + " " + string_join(2, s) + "\r\n");
}

void  Command::makePrivMessage(Client *client, std::string senderName, std::string receiver, std::string msg) //sendprivmassage로 바꾸기
{
	if (client == NULL)
		return ;
	client->appendMsgBuffer(":" + senderName + " PRIVMSG " + receiver + " " + msg + "\r\n");
}

void Command::channelMessage(std::string msg, Client *client, Channel *channel) // sendmessage channel
{
	std::vector<int> clientsInChannel = channel->getClientFdList();
	std::vector<int>::iterator clientsInChannelIt = clientsInChannel.begin();
	while (clientsInChannelIt != clientsInChannel.end())
	{
		if (client->getClientFd() != (*clientsInChannelIt))
			makePrivMessage(_server->findClient(*clientsInChannelIt), client->getNickName(), channel->getChannelName(), msg);
		clientsInChannelIt++;
	}
}

void Command::part(std::vector<std::string> s, Client *client)
{
	if (s.size() < 2)
	{
		makeNumericReply(client->getClientFd(), ERR_NEEDMOREPARAMS, "PART :Not enough parameters");
	}
	std::vector<std::string> partChannel = ft_split(s[1], ",");
	std::vector<std::string>::iterator partChannelIt = partChannel.begin();
	while (partChannelIt != partChannel.end())
	{
		std::vector<std::string>::iterator clientPartChannelIt = client->findMyChannelIt(*partChannelIt);
		if (clientPartChannelIt != client->getMyChannelList().end())
		{
			allInChannelMsg(client->getClientFd(), *clientPartChannelIt, "PART", string_join(2, s));
			Channel *tmp = _server->findChannel(*clientPartChannelIt);
			tmp->removeClientList(client->getClientFd());
			client->removeMyChannelIt(clientPartChannelIt);

			if (tmp->getClientFdList().empty())
			{
				_server->getChannelList().erase(tmp->getChannelName());
				delete tmp;
			}
			else
			{
				tmp->setChannelOperator(*(tmp->getClientFdList().begin()));
			}
		}
		else
		{
			if (_server->getChannelList().find(*partChannelIt) == _server->getChannelList().end())
				makeNumericReply(client->getClientFd(), ERR_NOSUCHCHANNEL, *partChannelIt + " :No such channel");
			else
				makeNumericReply(client->getClientFd(), ERR_NOTONCHANNEL, *partChannelIt + " :You're not on that channel");
		}
		partChannelIt++;
	}
}

void Command::quit(std::vector<std::string> s, Client *client)
{
	std::vector<std::string>::iterator channelListInClientIt = client->getMyChannelList().begin();
	while (channelListInClientIt != client->getMyChannelList().end())
	{
		Channel *tmpChannel = _server->findChannel(*channelListInClientIt);
		tmpChannel->removeClientList(client->getClientFd());
		allInChannelMsg(client->getClientFd(), tmpChannel->getChannelName(), "PART", string_join(1, s));
		if (tmpChannel->getClientFdList().empty() == true)
		{
			_server->getChannelList().erase(tmpChannel->getChannelName());
			delete tmpChannel;
		}
		else 
		{
			tmpChannel->setChannelOperator(*(tmpChannel->getClientFdList().begin()));
		}
		channelListInClientIt++;
	}
	_server->getClientList().erase(client->getClientFd());
	close(client->getClientFd());
	delete client;
}

std::string Command::makeFullname(int fd)
{
	Client *client = _server->findClient(fd);
	std::string fullname = (":" + client->getNickName() + "!" + client->getUserName() + "@" + client->getServerName());
	return (fullname);
}

