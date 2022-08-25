#include <iostream>
#include <map>
#include <sys/time.h>
#include <unistd.h>

class Human
{
public:
	virtual int createMan()
	{
		return 0;
	}

	int createWoman()
	{
		return 1;
	}
};

class MicrosoftHuman : public Human
{
	int createMan()
	{
		return 1;
	}
};

int main(void)
{

	Human *human = new MicrosoftHuman();

	std::cout << human->createMan() << std::endl;

	return (0);
}