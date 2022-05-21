
#include <map>
#include <iostream>
#include <sstream>

void test(std::ostringstream oss)
{
	std::cout << oss.str() << std::endl;
}

int main()
{

	test(std::ostringstream() << "vye bye");

	return (0);
}