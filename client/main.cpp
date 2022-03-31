#include "../src/socket.hpp"

#include <iostream>

int main(void)
{
	try
	{
		Socket client(IP, PORT, CLIENT);
		client.connecting();

		char buffer[1024];

		int n = recv(client.fd(), buffer, 1024, 0);
		buffer[n] = '\0';

		std::cout << buffer << std::endl;


		std::string message;

		std::cin >> message;

		std::cout << message;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}