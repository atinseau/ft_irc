#include "../command.hpp"


void Command::info(Payload p)
{
	Client& client = p.client;
	std::vector<std::string> infos;
	char buffer[INET6_ADDRSTRLEN];

	inet_ntop(AF_INET6, &(Server::address.sin6_addr), buffer, INET6_ADDRSTRLEN);
	
	infos.push_back("------------------ SERVER INFO ------------------");
	infos.push_back("Nom du serveur: " + Server::name);
	infos.push_back("Adresse du serveur: " + std::string(buffer));
	infos.push_back("Port du serveur: " + std::to_string(Server::port));
	infos.push_back("Nombre de clients: " + std::to_string(Server::clients.size()));
	infos.push_back("------------------ CLIENT INFO ------------------");
	
	std::vector<std::string> client_infos = client.get_info();
	for (std::vector<std::string>::iterator it = client_infos.begin(); it != client_infos.end(); it++)
		infos.push_back(*it);
	
	for (std::vector<std::string>::const_iterator it = infos.begin(); it != infos.end(); it++)
		client.write(RPL_INFO(client.get_key("USERNAME"), *it));
}