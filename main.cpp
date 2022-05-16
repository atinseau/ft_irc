#include "src/irc.hpp"

void app()
{
	Server srv;
	srv.run();
}

int main(int argc, const char **argv, const char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;

	try
	{
		app();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
