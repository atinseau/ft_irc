#include <iostream>
#include <vector>


std::vector<std::string> split(const char *str, char c)
{
	std::vector<std::string> res;
	std::string dup;
	size_t pos = 0;

	if (!str)
		return res;
	dup.append(str);
	while ((pos = dup.find(c)) != std::string::npos)
	{
		res.push_back(dup.substr(0, pos));
		dup.erase(0, pos + 1);
	}
	res.push_back(dup);
	return res;
}



int main(void)
{
	std::vector<std::string> vec = split("Hello World\nHello World\nqsdqsqdqsd", '\n');

	for (auto&i : vec)
		std::cout << i << std::endl;

	return (0);
}