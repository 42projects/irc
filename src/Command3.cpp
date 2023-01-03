#include "Server.hpp"
#include "Command.hpp"

//새로 들어온 유저를 환영하는 메시지를 보낸다. 
void Command::welcomeMsg(int fd, std::string flag, std::string msg, std::string name)
{
	Client *c = _server->findClient(fd);

	c->appendMsgBuffer(flag);
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(name);
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(msg);
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(name);
	c->appendMsgBuffer("\r\n");
}

//전체 클라이언트들에게 메시지를 보낸다.
void Command::allInChannelMsg(int target, std::string channelName, std::string command, std::string msg)
{
	Channel *channel = _server->findChannel(channelName);
	std::vector<int> myClientList = channel->getClientFdList();
	std::vector<int>::iterator it = myClientList.begin();
	while (it < myClientList.end())
	{
		Client *tmp = _server->findClient(*it);
		tmp->appendMsgBuffer(makeFullname(target) + " " + command + " " + channelName + " " + msg + "\r\n");
		it++;
	}
}

void Command::nameListMsg(int fd, std::string channelName)
{
	Client *tmp = _server->findClient(fd);
	tmp->appendMsgBuffer(RPL_NAMREPLY);
	tmp->appendMsgBuffer(" ");
	tmp->appendMsgBuffer(tmp->getNickName());
	tmp->appendMsgBuffer(" = " + channelName);

	Channel *targetChannel = _server->findChannel(channelName);
	std::vector<int> clientList = targetChannel->getClientFdList();
	std::vector<int>::iterator it =clientList.begin();
	std::string name;
	tmp->appendMsgBuffer(" :");

	for (; it < clientList.end() - 1; it++)
	{
		if (targetChannel->getChannelOperator() == *it)
			tmp->appendMsgBuffer("@");
		name = (_server->findClient(*it)->getNickName());
		tmp->appendMsgBuffer(name);
		tmp->appendMsgBuffer(" ");
	}
	if (targetChannel->getChannelOperator() == *it)
		tmp->appendMsgBuffer("@");
	name = (_server->findClient(*it)->getNickName());
	tmp->appendMsgBuffer(name);
	tmp->appendMsgBuffer("\r\n");
	
	tmp->appendMsgBuffer(RPL_ENDOFNAMES);
	tmp->appendMsgBuffer(" " + tmp->getNickName() + " " + channelName + " :End of NAMES list" + "\r\n");
}

void Command::makeNumericReply(int fd, std::string flag, std::string str)
{
	Client *c = _server->findClient(fd);

	c->appendMsgBuffer(flag);
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(c->getNickName());
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(str);
	c->appendMsgBuffer("\r\n");
}

void Command::makeCommandReply(int fd, std::string command, std::string str)
{
	Client *c = _server->findClient(fd);

	c->appendMsgBuffer(":");
	c->appendMsgBuffer(c->getNickName());
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(command);
	c->appendMsgBuffer(" ");
	c->appendMsgBuffer(str);
	c->appendMsgBuffer("\r\n");
}

void Command::welcome(std::vector<std::string> cmd, Client *client, std::map<int, Client *> clientList)
{
	std::vector<std::string>::iterator it = cmd.begin();

	while (it != cmd.end())
	{
		//PASS -> NICK -> USER 순서로 입력이 들어올 때만 회원가입이 정상적으로 진행된다.
		std::vector<std::string> temp = ft_split(*it, " ");
		if (temp[0] == "PASS")
		{
			if (client->getRegist() & PASS)
			{
				makeNumericReply(client->getClientFd(), ERR_ALREADYREGISTRED, ":Already checked password");
				return ;
			}
			if (temp.size() == 1)
			{
				makeNumericReply(client->getClientFd(), ERR_NEEDMOREPARAMS, "PASS :Not enough parameter\n/PASS <password> format");
				return ;
			}
			if (temp[1] == _server->getPass())
			{
				client->setRegist(PASS);
			}
			else
			{
				makeNumericReply(client->getClientFd(), ERR_PASSWDMISMATCH, ":Wrong Password");
				_server->removeUnconnectClient(client->getClientFd());
				return ;
			}

		}
		else if ((client->getRegist() & PASS) && (temp[0] == "NICK"))
		{
			if (!nickValidate(temp[1]))
			{
				makeNumericReply(client->getClientFd(), ERR_ERRONEUSNICKNAME, ":Improper Nickname");
				return ;
			}
			if (isDuplication(temp[1], clientList))
			{
				if (client->getNickName() == temp[1])
					return ;
				temp[1].append("_");
			}
			client->setNickName(temp[1]);
			client->setRegist(NICK);
			makeCommandReply(client->getClientFd(), "NICK", temp[1]);
		}
		else if ((client->getRegist() & PASS) && (client->getRegist() & NICK) && (temp[0] == "USER"))
		{
			if (temp.size() < 5)
			{
				makeNumericReply(client->getClientFd(), ERR_NEEDMOREPARAMS, "USER :Not enough parameter\n/USER <username> <hostname> <servername> <:realname>");
				return ;
			}
			client->setUser(temp[1], temp[2], temp[3], string_join(4, temp));
			client->setRegist(USER);
		}
		else if (temp[0] != "CAP")	//그 외의 경우에는 확인을 통해 에러 출력.
		{
			if (!(client->getRegist() & PASS))
			{
				makeNumericReply(client->getClientFd(), ERR_NOTREGISTERED, ":You have not registered Server's Password");
				_server->removeUnconnectClient(client->getClientFd());
			}
			else if (!(client->getRegist() & NICK))
				makeNumericReply(client->getClientFd(), ERR_NOTREGISTERED, ":You have not registered set Nickname");
			else if (!(client->getRegist() & USER))
				makeNumericReply(client->getClientFd(), ERR_NOTREGISTERED, ":You have not registered USER info");
			return ;
		}
		it++;
	}
	if ((client->getRegist() & PASS) && (client->getRegist() & NICK) && (client->getRegist() & USER))
	{
		welcomeMsg(client->getClientFd(), RPL_WELCOME, ":Welcome to Internet Relay Network", client->getNickName());
		client->setRegist(REGI);
	}
}

void Command::alreadyRegist(Client *client)
{
	makeNumericReply(client->getClientFd(), ERR_ALREADYREGISTRED, ":You are already registed");
}