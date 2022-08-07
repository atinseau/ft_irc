#include <iostream>
#include <vector>

std::string itoa (int i)
{
	std::string res;
	std::string reverse;
	int e;

	if (i < 0)
	{
		reverse += '-';
		i *= -1;
	}
	while (i)
	{
		res += i % 10 + '0';
		i /= 10;
	}
	e = res.size();
	while (e--)
		reverse += res[e];
	return reverse;
}

void pop_back(std::string& str)
{
	str.erase(str.size() - 1);
}

int main(void)
{
	std::string a = itoa(-267);

	pop_back(a);

	std::cout << a << std::endl;

	return (0);
}