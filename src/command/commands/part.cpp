#include "../command.hpp"

void Command::part(Payload p)
{

	if (!p.body.second.size() || p.body.second[0] == ":")
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), "PART"));

	std::vector<std::string> channels = utils::split(p.body.second[0].c_str(), ',');

	for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::string channel_name = fix_channel_name(*it);
		if(!p.client.leave(channel_name))
		{
			p.client.write(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), channel_name));
			continue;
		}
		std::map<std::string, Channel>::iterator channel_it = Server::channels.find(channel_name);
		if (channel_it == Server::channels.end() || channel_it->second.connected_clients() > 0)
			continue;
		Server::channels.erase(channel_it);
	}

}