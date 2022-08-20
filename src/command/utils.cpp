#include "command.hpp"


void say_hello(Client& client)
{
	const std::string& nickname = client.get_key("NICKNAME");

	client.write(RPL_WELCOME(nickname));
	client.write(RPL_YOURHOST(nickname));
	client.write(RPL_CREATED(nickname, utils::format_time(Server::launch_time)));
}		