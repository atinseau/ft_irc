#include "src/irc.hpp"

void app(const char **envp)
{
	Socket server(IP, PORT, SERVER, envp);

	server.setPassword(PASSWORD);
	server.listening();
	server.run();

}

int main(int argc, const char **argv, const char **envp)
{
	(void)argc;
	(void)argv;

	try
	{
		app(envp);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}