/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:38 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 20:13:29 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

using namespace std;

Command::Command(Client& client, std::vector<Client>& clients): _client(client), _clients(clients) {}

void Command::ex_cmd(const Request::Body& body, std::map<std::string, Channel>& channels)
{
	(void) _client;
	(void) _clients;

	INFO("on pass par ex_cmd");
	map_t::iterator it = _commands.find(body.first);

	if (it != _commands.end())
	{
		if (!_client.is_auth())
		{
			if (it->first != "PASS" && it->first != "NICK" && it->first != "USER")
			{
				ERROR("Commande non autorisée");
				return;
			}
		}
		else
			INFO("Authentifié");
		it->second(Payload(_client, _clients, body, channels));
		return;
	}
	ERROR("Commande " << body.first << " inconnue");
}


void Command::nick(Payload p)
{
	(void)p;
	// if (p.body.second.size() == 0)
	// {
	// 	if (!p.client.is_auth())
	// 		throw AuthException(ERR_NONICKNAMEGIVEN); 
	// 	throw ResponseException(ERR_NONICKNAMEGIVEN);
	// }

	// std::string nickname = p.body.second[0];
	// if (nickname[0] == ':')
	// 	nickname.erase(0);

	// if (nickname.size() > NICKNAME_LENGTH) // todo: check if nickname is valid
	// {
	// 	if (!p.client.is_auth())
	// 		throw AuthException(ERR_ERRONEUSNICKNAME(nickname));
	// 	throw ResponseException(ERR_ERRONEUSNICKNAME(nickname));
	// }

	// for (std::vector<Client>::const_iterator it = p.clients.begin(); it != p.clients.end(); it++)
	// {
	// 	if (it->get_fd() != -1 && it.base() != &p.client && it->get_key("NICKNAME") == nickname)
	// 	{
	// 		if (!p.client.is_auth())
	// 			throw AuthException(ERR_NICKNAMEINUSE(nickname));
	// 		throw ResponseException(ERR_NICKNAMEINUSE(nickname));
	// 	}
	// }


	// p.client["NICKNAME"] = nickname;
	// WARNING("Nickname is now: " << p.client["NICKNAME"]);
}

void Command::pass(Payload p)
{
	(void)p;
	// if (p.client["PASSWORD"].size() > 0)
	// {
		
	// 	return;
	// }
	// if (p.body.second.size() > 1)
	// {
		
	// 	return;
	// }

	// if (p.body.second[0] != Client::server_password)
	// {
	// 	return;
	// }
		
	// p.client["PASSWORD"] = p.body.second[0];
}

void Command::user(Payload p)
{
	(void)p;
	// if (p.body.second.size() != 4)
	// {
	// 	return;
	// }

	// p.client["USERNAME"] = p.body.second[0];
	// p.client["REALNAME"] = p.body.second[3];

	// WARNING("Username is now: " << p.client["USERNAME"]);
}

void Command::join(Payload p)
{
	(void)p;
	// std::vector<std::string> body_channel;
	// std::vector<std::string> body_para;
	// SUCCESS("on a bien appeler join");
	// for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it < p.body.second.end(); it++)
	// {
	// 	std::cout << "coucou" << std::endl;
	// 	if ((*it)[0] == '#')
	// 		body_channel.push_back(*it);
	// 	else 
	// 		body_para.push_back(*it);
	// }
	// for (size_t i = 0; i < p.body.second.size(); i++)
	// {
	// 	if (p.channels.find(body_channel[i]) != p.channels.end())
	// 	{
	// 		SUCCESS("channel localiser");
	// 		char c = p.channels[body_channel[i]].get_mode();
	// 		if (c != 'i' && c != 'p' && c != 's')
	// 		{
	// 			if (c == 'k' && i < body_para.size()
	// 				&& body_para[i] == p.channels[body_channel[i]].get_password())
	// 				p.client.add_channel(&(p.channels[p.body.second[i]]));
	// 			else if (i >= body_para.size() || body_para[i] != p.channels[body_channel[i]].get_password())
	// 				throw ResponseException(ERR_PASSCHANNEL(p.client.get_key("NICKNAME")));
	// 			else
	// 				p.client.add_channel(&(p.channels[p.body.second[i]]));
	// 		}
	// 		throw ResponseException(ERR_CHANNELISNOTAVAILABLE(p.client.get_key("NICKNAME")));
	// 	}
	// 	else
	// 		throw ResponseException(ERR_CHANNELDOESNOTEXIST(p.client.get_key("NICKNAME")));
	// }
}

Command::map_t Command::init()
{
	map_t map;

	map["NICK"] = &Command::nick;
	map["PASS"] = &Command::pass;
	map["USER"] = &Command::user;
	map["JOIN"] = &Command::join;

	return (map);
}
Command::map_t Command::_commands = Command::init();
