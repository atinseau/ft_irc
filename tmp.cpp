
#include <map>
#include <iostream>

int main()
{

	std::map<std::string, std::string> map;

	map["USERNAME"] = "a";
	map["NICKNAME"] = "b";
	map["CHANNEL"] = "c";

	std::cout << map["OK"] << std::endl;

	return (0);
}