#include "../command.hpp"


void Command::kick(Payload& p)
{
	if (p.body.second.size() < 2)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), "KICK"));

	std::string channel = fix_channel_name(p.body.second[0]);
	std::string target = p.body.second[1];

	std::map<std::string, Channel>::iterator it = Server::channels.find(channel);
	if (it == Server::channels.end())
		throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), channel));

	Operator* op = it->second.get_operator(p.client);
	if (!op)
		throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), channel));
	if (!op->has('o'))
		throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), channel));

	Client* client = get_client_by_key("NICKNAME", target.c_str());
	if (!client)
		throw ResponseException(ERR_NOSUCHNICK(p.client.get_key("NICKNAME"), target));

	Channel::Dispatcher dispatcher = it->second.create_dispatcher();

	dispatcher.load(RPL_KICK(p.client.fullname(), channel, client->get_key("NICKNAME")));
	dispatcher.send();

	it->second.kick(*client);
}