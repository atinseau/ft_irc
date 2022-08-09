#include "../command.hpp"

void Command::part(Payload p)
{

	if (!p.body.second.size() || p.body.second[0] == ":")
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("USERNAME"), "PART"));

	for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it != p.body.second.end(); it++)
	{
		std::string channel_name = fix_channel_name(*it);
		if(!p.client.leave(channel_name))
		{
			p.client.write(ERR_NOTONCHANNEL(channel_name, p.client.get_key("USERNAME")));
			continue;
		}
		p.client.write(RPL_PART(p.client.get_key("USERNAME"), channel_name));
		std::map<std::string, Channel>::iterator channel_it = Server::channels.find(channel_name);
		if (channel_it == Server::channels.end() || channel_it->second.connected_clients() > 0)
			continue;
		Server::channels.erase(channel_it);
	}

}