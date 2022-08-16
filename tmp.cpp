#include <iostream>
#include <map>
#include <sys/time.h>
#include <unistd.h>

bool is_a (char c)
{
	if (c == 'a')
		return true;
	return false;
}

bool is_b (char c)
{
	if (c == 'b')
		return true;
	return false;
}


int main(void)
{
	typedef bool (*func_t)(char);
	typedef func_t func_t_array[];

	func_t_array f = {
		is_a,
		is_b,
		is_a
	};



	std::cout << sizeof(f) / sizeof(func_t) << std::endl;
	
	return (0);
}