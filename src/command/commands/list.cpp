#include "../command.hpp"

void send_responses(std::vector<std::string>& responses, Client& client)
{
	const std::string& nickname = client.get_key("NICKNAME");

	client.write(RPL_LISTSTART(nickname));
	for (std::vector<std::string>::iterator it = responses.begin(); it != responses.end(); it++)
		client.write(*it);
	client.write(RPL_LISTEND(nickname));
}

void Command::list(Payload p)
{
	const std::string& nickname = p.client.get_key("NICKNAME");
	bool by_key = p.body.second.size() > 0 && p.body.second[0] != "";

	std::vector<std::string> responses;
	for (std::map<std::string, Channel>::iterator it = Server::channels.begin(); it != Server::channels.end(); it++)
	{
		if (by_key && utils::find(p.body.second.begin(), p.body.second.end(), it->second.get_name()) == p.body.second.end())
			continue;
		responses.push_back(RPL_LIST(nickname, it->second.get_name(), itoa(it->second.connected_clients()), it->second.get_topic()));
	}

	if (!Server::channels.size() || !responses.size())
		throw ResponseException(ERR_NOCHANNELS(nickname));
	send_responses(responses, p.client);
}