#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <iostream>

std::string					string_join(size_t startIndex, std::vector<std::string> input);
std::vector<std::string>	ft_split(std::string &str, std::string s);
void						printStringVector(std::vector<std::string> input);

#endif