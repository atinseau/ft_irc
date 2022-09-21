#include "../command.hpp"

void Command::names(Payload& payload)
{
	Client& client = payload.client;

	if (!payload.body.second.size())
	{
		client.write(RPL_ENDOFNAMES(client.get_key("NICKNAME"), "*"));
		return;
	}

	std::string channel_name = fix_channel_name(payload.body.second[0]);
	std::map<std::string, Channel>::iterator it = Server::channels.find(channel_name);
	if (it == Server::channels.end())
	{
		client.write(ERR_NOSUCHCHANNEL(client.get_key("NICKNAME"), channel_name));
		return;
	}

	if (it->second.has('s'))
	{
		client.write(ERR_NOSUCHCHANNEL(client.get_key("NICKNAME"), channel_name));
		return;
	}

	if (it->second.has('p') && !it->second.include(client))
	{
		client.write(ERR_NOTONCHANNEL(client.get_key("NICKNAME"), channel_name));
		return;
	}

	client.write(RPL_NAMREPLY(client.get_key("NICKNAME"), it->second.get_name(), it->second.get_client_list()));
	client.write(RPL_ENDOFNAMES(client.get_key("NICKNAME"), it->second.get_name()));
}