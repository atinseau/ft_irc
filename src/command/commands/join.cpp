#include "../command.hpp"

Channel create_own_channel(std::string name, std::string password, Client& client)
{
	Channel channel(name, password);
	Operator& op = channel.insert(client);
	op.give("o");
	return channel;
}

void join_channel(Channel& channel, const std::string& password, Client& client)
{
	if (channel.get_password().size() && channel.get_password() != password)
		throw ResponseException(ERR_BADCHANNELKEY(channel.get_name(), client.get_key("USERNAME")));
	if (channel.include(client.get_fd()))
		throw ResponseException(ERR_USERONCHANNEL(channel.get_name(), client.get_key("USERNAME")));
	channel.insert(client);
}

void Command::join(Payload p)
{
	if (p.body.second.size() < 1 || (p.body.second.size() == 1 && p.body.second[0] == "#"))
		throw ResponseException(ERR_NEEDMOREPARAMS("*", "JOIN"));

	std::vector<std::string> channel_keys = split(p.body.second[0].c_str(), ',');
	std::vector<std::string> channel_password = split(p.body.second.size() >= 2 ? p.body.second[1].c_str() : NULL, ',');

	utils::remove_if(channel_keys, "");
	utils::remove_if(channel_password, "");
	
	for (std::vector<std::string>::iterator it = channel_keys.begin(); it != channel_keys.end(); it++)
	{
		std::map<std::string, Channel>::iterator channel_it = Server::channels.find(*it);
		std::string password = channel_password.size() == channel_keys.size() ? channel_password[it - channel_keys.begin()] : "";

		if (it->at(0) != '#' && it->at(0) != '&')
			it->insert(it->begin(), '#');

		if (it->size() > 200)
			throw ResponseException(ERR_NOSUCHCHANNEL(*it, p.client.get_key("USERNAME")));


		if (channel_it == Server::channels.end())
		{
			Channel channel = create_own_channel(*it, password, p.client);
			Server::channels.insert(std::make_pair(*it, channel));
			continue;
		}
		join_channel(channel_it->second, password, p.client);
	}

}
