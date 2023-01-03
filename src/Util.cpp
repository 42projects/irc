#include "Util.hpp"

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

std::vector<std::string> ft_split(std::string &line, std::string s)
{
	std::vector<std::string> tab;
	std::string cmd_buf;
	size_t start = 0;
	size_t pos;

	while ((pos = line.find(s)) != std::string::npos){
		tab.push_back(line.substr(start, pos));
		line = line.substr(pos + s.length());
	}
	if (line != "")
		tab.push_back(line);

	return (tab);
}

void	printStringVector(std::vector<std::string> input)
{
	std::vector<std::string>::iterator it = input.begin();

	std::cout << "----------in print_stringVector\n";
	while (it != input.end())
	{
		std::cout << "[" << *it << "]" << std::endl;
		it++;
	}
	std::cout << "ㄴ------------------\n\n";
}
