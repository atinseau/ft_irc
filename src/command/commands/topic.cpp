#include "../command.hpp"

void Command::topic(Payload p)
{
	if (!p.body.second.size())
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), "TOPIC"));

	std::string channel_name = fix_channel_name(p.body.second[0]);
	std::map<std::string, Channel>::iterator it = Server::channels.find(channel_name);
	if (it == Server::channels.end())
		throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), channel_name));
	
	if (p.body.second.size() == 1)
	{
		if (it->second.get_topic().empty())
			p.client.write(RPL_NOTOPIC(p.client.get_key("NICKNAME"), channel_name));
		else
			p.client.write(RPL_TOPIC(p.client.get_key("NICKNAME"), channel_name, it->second.get_topic()));
		return;
	}

	Operator* op = it->second.get_operator(p.client.get_fd());
	if (it->second.has('t') && !op->has('o'))
		throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), channel_name));
	it->second.set_topic(utils::join(p.body.second, " ", 1));
	
	Channel::Dispatcher dispatcher = it->second.create_dispatcher();

	dispatcher.load(RPL_TOPIC(p.client.get_key("NICKNAME"), channel_name, it->second.get_topic()));
	dispatcher.send();
}