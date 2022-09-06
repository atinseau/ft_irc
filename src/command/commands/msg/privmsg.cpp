#include "./msg.hpp"


void Command::privmsg(Payload& p)
{

	Client &client = p.client;

	if (p.body.second.size() < 2)
		throw ResponseException(ERR_NEEDMOREPARAMS(client.get_key("NICKNAME"), "PRIVMSG"));

	std::vector<Recipient> recipients = get_recipients(p.body.second[0]);
	std::string message = get_message(p.body.second);

	const std::string& from = client.get_key("NICKNAME");
	for (std::vector<Recipient>::iterator it = recipients.begin(); it != recipients.end(); it++)
	{
		if (it->is_channel)
		{
			std::map<std::string, Channel>::iterator channel_it = Server::channels.find(it->identifier);
			if (channel_it == Server::channels.end())
			{
				client.write(ERR_NOSUCHCHANNEL(from, it->identifier));
				continue;
			}
			if (channel_it->second.has('n'))
			{
				if (channel_it->second.has('s'))
					client.write(ERR_NOSUCHCHANNEL(from, it->identifier));
				else
					client.write(ERR_CANNOTSENDTOCHAN(from, it->identifier));
				continue;
			}

			Operator* op = channel_it->second.get_operator(client);
			if (channel_it->second.has('m') && (!op->has('o') && !op->has('v')))
			{
				client.write(ERR_CANNOTSENDTOCHAN(from, it->identifier));
				continue;
			}

			Channel::Dispatcher dispatcher = channel_it->second.create_dispatcher(&client);
			dispatcher.load(RPL_PRIVMSG(client.fullname(), it->identifier, message));
			dispatcher.send(is_not_target);
		}
		else
		{
			Client *recipient = get_client_by_key("NICKNAME", it->identifier.c_str());
			if (recipient == NULL)
				client.write(ERR_NOSUCHNICK(from, it->identifier));
			else
				recipient->write(RPL_PRIVMSG(client.fullname(), it->identifier, message));
		}
	}
}