#include <sys/time.h>
#include <iostream>

int main()
{

	struct timeval tv;

	gettimeofday(&tv, NULL);

	std::cout << tv.tv_sec << "." << tv.tv_usec << std::endl;

	return (0);
}