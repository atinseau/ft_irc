#include "../command.hpp"

void Command::names(Payload& payload)
{
	(void)payload;

	Client& client = payload.client;

	Channel& channel = Server::channels.find("#salut")->second;

	client.write(RPL_NAMREPLY(client.get_key("NICKNAME"), channel.get_name(), client.fullname()));
	client.write(RPL_ENDOFNAMES(client.get_key("NICKNAME"), channel.get_name()));
}