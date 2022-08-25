#include "../command.hpp"

void Command::invite(Payload& p)
{
	(void)p;

	if (p.body.second.size() != 2)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), "INVITE"));

	std::string nickname = p.body.second[0];
	std::string channel_name = fix_channel_name(p.body.second[1]);

	Client *target = get_client_by_key("NICKNAME", nickname.c_str());
	if (!target)
		throw ResponseException(ERR_NOSUCHNICK(p.client.get_key("NICKNAME"), nickname));

	std::map<std::string, Channel>::iterator it = Server::channels.find(channel_name);
	if (it == Server::channels.end())
	{
		target->write(RPL_INVITE(p.client.fullname(), channel_name, target->get_key("NICKNAME")));
		return;
	}

	Channel& channel = it->second;
	Operator* op = channel.get_operator(p.client);

	if (!op)
		throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), channel_name));
	if (channel.has('i') && !op->has('o'))
		throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), channel_name));
	channel.invite(*target);
	target->write(RPL_INVITE(p.client.fullname(), channel_name, target->get_key("NICKNAME")));
}