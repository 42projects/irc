#include "Command.hpp"

Command::Command(Server *server)
{
	_server = server;
}

Command::~Command(){}

bool Command::isLetter(char c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return true;
	else
		return false;
}

bool Command::isNumber(char c)
{
	if ('0' <= c && c <= '9')
		return true;
	else
		return false;
}

bool Command::isSpecial(char c)
{
	if (c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' || c == '{' || c == '}')
		return true;
	else
		return false;
}

bool Command::isDuplication(std::string s, std::map<int, Client *> clientList)
{
	std::map<int, Client *>::iterator it = clientList.begin();
	while (it != clientList.end())
	{
		if (it->second->getNickName() == s)
			return true;
		it++;
	}
	return false;
}

bool Command::nickValidate(std::string s)
{
	if (0 >= s.length() || s.length() > 9)
		return false;
	if (isLetter(s[0]) == false)
		return false;
	for (int i = 1; i < (int)(s.length()); i++)
	{
		if (!isLetter(s[i]) && !isSpecial(s[i]) && !isNumber(s[i]))
			return false;
	}
	return true;
}