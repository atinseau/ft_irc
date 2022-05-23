/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:38 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 12:38:48 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

using namespace std;

Command::Command(Client& client, std::vector<Client>& clients): _client(client), _clients(clients) {}

void Command::operator[](const Request::Body& body)
{
	(void) _client;
	(void) _clients;

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
		it->second(Payload(_client, _clients, body));
		return;
	}
	ERROR("Commande " << body.first << " inconnue");
}


void Command::nick(Payload p)
{
	if (p.body.second.size() == 0)
	{
		if (!p.client.is_auth())
			throw AuthException(ERR_NONICKNAMEGIVEN); 
		throw ResponseException(ERR_NONICKNAMEGIVEN);
	}

	std::string nickname = p.body.second[0];
	if (nickname[0] == ':')
		nickname.erase(0);

	if (nickname.size() > NICKNAME_LENGTH) // todo: check if nickname is valid
	{
		if (!p.client.is_auth())
			throw AuthException(ERR_ERRONEUSNICKNAME(nickname));
		throw ResponseException(ERR_ERRONEUSNICKNAME(nickname));
	}

	for (std::vector<Client>::const_iterator it = p.clients.begin(); it != p.clients.end(); it++)
	{
		if (it->get_fd() > 0 && it.base() != &p.client && it->get_key("NICKNAME") == nickname)
		{
			if (!p.client.is_auth())
				throw AuthException(ERR_NICKNAMEINUSE(nickname));
			throw ResponseException(ERR_NICKNAMEINUSE(nickname));
		}
	}

	p.client["NICKNAME"] = nickname;
	WARNING("Nickname is now: " << p.client["NICKNAME"]);
}

void Command::pass(Payload p)
{
	if (p.client["PASSWORD"].size() > 0)
	{
		
		return;
	}
	if (p.body.second.size() > 1)
	{
		
		return;
	}

	if (p.body.second[0] != Client::server_password)
	{
		return;
	}
		
	p.client["PASSWORD"] = p.body.second[0];
}

void Command::user(Payload p)
{

	if (p.body.second.size() != 4)
	{
		return;
	}

	p.client["USERNAME"] = p.body.second[0];
	p.client["REALNAME"] = p.body.second[3];

	WARNING("Username is now: " << p.client["USERNAME"]);
}

void Command::join(Payload p)
{
	(void) p;
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
