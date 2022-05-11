#include "../src/irc.hpp"

#include <iostream>

// SHELL_CLIENT

int main(int argc, const char **argv, const char **envp)
{
	(void)argv;
	(void)argc;
	(void)envp;

	try
	{
		Socket client(IP, PORT, CLIENT);
		client.connecting();

		loop {
			std::string message = "";
			std::getline(std::cin, message);
			if (message == "exit")
				break;
			send(client.fd(), message.c_str(), message.size(), 0);
		}
		

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}