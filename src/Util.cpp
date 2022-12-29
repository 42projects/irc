#include "../include/Util.hpp"

std::string string_join(size_t startIndex, std::vector<std::string> input)
{
	std::string result = "";
	for (size_t i = startIndex; i < input.size() - 1; i++)
	{
		result.append(input[i]);
		result.append(" ");
	}
	result.append(input[input.size() - 1]);
	return result;
}

std::vector<std::string> ft_split(std::string str, std::string s)
{
	std::vector<std::string>	ret;
	int							prev = 0;
	int							current = 0;

	current = str.find(s);
	while (current != std::string::npos)
	{
		std::string substring = str.substr(prev, current - prev);
		ret.push_back(substring);
		prev = current + 1;
		current = str.find(s, prev);
	}
	ret.push_back(str.substr(prev, current - prev));

	return (ret);
}

void	printStringVector(std::vector<std::string> input)
{
	std::vector<std::string>::iterator it = input.begin();

	while (it != input.end())
	{
		std::cout << "[" << *it << "]" << std::endl;
		it++;
	}
}
