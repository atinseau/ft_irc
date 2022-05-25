#include <sys/time.h>
#include <iostream>
#include <vector>

int main()
{
	int x = 10;
	int &i = x;
	std::vector<int &> test;
	test.push_back(i);
	//std::cout << *(test.begin()) << std::endl;
	//test.erase(test.begin());
	//std::cout << "ici : " << i << std::endl;


	return (0);
}