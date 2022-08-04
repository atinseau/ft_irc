#include "command.hpp"


void say_hello(Client& client)
{
	const std::string& username = client.get_key("USERNAME");

	client.write(RPL_WELCOME(username));
	client.write(RPL_YOURHOST(username));
	client.write(RPL_CREATED(username, format_time(Server::launch_time)));
}		