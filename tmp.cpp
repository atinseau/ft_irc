#include <sys/time.h>
#include <iostream>
#include <vector>

int main()
{
	int x = 1;
	int i = 2;
	int y = 3;
	int v = 4;

	std::vector<int> test;
	test.push_back(x);
	//std::cout << &test.begin() << std::endl;
	std::cout << test.begin().base() << std::endl;
	std::cout << (test.end() - 1).base() << std::endl<< std::endl;
	test.push_back(i);
	//std::cout << &test.begin() << std::endl;
	std::cout << test.begin().base() << std::endl;
	std::cout << (test.end() - 1).base() << std::endl << std::endl;

	test.push_back(y);
	//std::cout << &test.begin() << std::endl;
	std::cout << test.begin().base() << std::endl;
	std::cout << (test.end() - 2).base() << std::endl;
	std::cout << (test.end() - 1).base() << std::endl<< std::endl;
	test.push_back(v);
	//std::cout << &test.begin() << std::endl;
	std::cout << test.begin().base() << std::endl;
	std::cout << (test.end() - 3).base() << std::endl;
	std::cout << (test.end() - 2).base() << std::endl;
	std::cout << (test.end() - 1).base() << std::endl<< std::endl;

	return (0);
}