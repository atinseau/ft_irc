#include <sys/time.h>
#include <iostream>
#include <vector>
#include <map>

int main()
{
	std::pair<int,char> a(1,'a');
	std::pair<int,char> b(2,'a');
	std::pair<int,char> c(3,'a');
	std::pair<int,char> d(4,'a');
	std::pair<int,char> e(5,'a');

	std::map<int, char> test;
	test.insert(a);
	for (std::map<int,char>::iterator it= test.begin(); it != test.end(); it++)
	{
		std::cout << "first  : " << &(it->first) << std::endl;
		std::cout << "second : " << it->second << std::endl;
	}
	std::cout << std::endl;
	test.insert(b);
	for (std::map<int,char>::iterator it= test.begin(); it != test.end(); it++)
	{
		std::cout << "first  : " << &(it->first) << std::endl;
		std::cout << "second : " << &(it->second) << std::endl;
	}
	std::cout << std::endl;

	test.insert(c);
	for (std::map<int,char>::iterator it= test.begin(); it != test.end(); it++)
	{
		std::cout << "first  : " << &(it->first) << std::endl;
		std::cout << "second : " << &(it->second) << std::endl;
	}
	std::cout << std::endl;

	test.insert(d);
	for (std::map<int,char>::iterator it= test.begin(); it != test.end(); it++)
	{
		std::cout << "first  : " << &(it->first) << std::endl;
		std::cout << "second : " << &(it->second) << std::endl;
	}
	std::cout << std::endl;

	test.insert(e);
	for (std::map<int,char>::iterator it= test.begin(); it != test.end(); it++)
	{
		std::cout << "first  : " << &(it->first) << std::endl;
		std::cout << "second : " << &(it->second) << std::endl;
	}



	
	return (0);
}