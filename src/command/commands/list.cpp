#include "../command.hpp"

void send_responses(std::vector<std::string>& responses, Client& client)
{
	std::string username = client.get_key("USERNAME");

	client.write(RPL_LISTSTART(username));
	for (std::vector<std::string>::iterator it = responses.begin(); it != responses.end(); it++)
		client.write(*it);
	client.write(RPL_LISTEND(username));
}

void Command::list(Payload p)
{
	std::string username = p.client.get_key("USERNAME");
	bool by_key = p.body.second.size() > 0 && p.body.second[0] != "";

	std::vector<std::string> responses;
	for (std::map<std::string, Channel>::iterator it = Server::channels.begin(); it != Server::channels.end(); it++)
	{
		if (by_key && utils::find(p.body.second.begin(), p.body.second.end(), it->second.get_name()) == p.body.second.end())
			continue;
		responses.push_back(RPL_LIST(username, it->second.get_name(), itoa(it->second.connected_clients()), it->second.get_topic()));
	}

	if (!Server::channels.size() || !responses.size())
		throw ResponseException(ERR_NOCHANNELS(username));
	send_responses(responses, p.client);
}