#include "src/socket.hpp"

void login(int client_fd) {
	
	send(client_fd, "login", 5, 0);
	
}



void app()
{
	Socket server(IP, PORT, SERVER);
	server.listening();
	server.create_client(login);
}


int main(void)
{
	try
	{
		app();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}