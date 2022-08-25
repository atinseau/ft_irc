#include "../command.hpp"

void Command::who(Payload& p)
{
	(void)p;
	// if (!p.body.second.size())
	// 	return;
	
	// std::string channel = fix_channel_name(p.body.second[0]);
	// std::map<std::string, Channel>::iterator it = Server::channels.find(channel);

	// if (it == Server::channels.end())
	// 	return;

	// Channel::Dispatcher dispatcher = it->second.create_dispatcher();

	// std::vector<int> clients = it->second.get_clients();
	// for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
	// {
	// 	std::map<int, Client>::iterator client_it = Server::clients.find(*it);
	// 	if (client_it == Server::clients.end())
	// 		continue;
	// 	dispatcher.load(RPL_WHOREPLY(p.client.get_key("NICKNAME"), client_it->second.get_key("NICKNAME"), channel, client_it->second.get_key("USERNAME"), client_it->second.get_key("REALNAME")));
	// }
	// dispatcher.load(RPL_ENDOFWHO(p.client.get_key("NICKNAME"), channel));
	// dispatcher.send();
}