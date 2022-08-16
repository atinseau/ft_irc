#include "../command.hpp"

struct Recipient
{
	std::string identifier;
	bool is_channel;
};

std::vector<Recipient> get_recipients(const std::string &payload)
{
	std::vector<Recipient> recipients;

	std::vector<std::string> tokens = split(payload.c_str(), ',');

	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		Recipient recipient;
		if (it->at(0) == '#' || it->at(0) == '&')
			recipient.is_channel = true;
		else
			recipient.is_channel = false;
		recipient.identifier = *it;
		recipients.push_back(recipient);
	}
	return (recipients);
}

std::string get_message(const std::vector<std::string> &vec)
{
	std::string message = join(vec, " ", 1);
	if (message[0] == ':')
		message.erase(0, 1);
	return (message);
}


void Command::privmsg(Payload p)
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
				client.write(ERR_NOTONCHANNEL(from, it->identifier));
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