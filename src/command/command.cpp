/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:38 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/06 08:28:18 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

using namespace std;

Command::Payload::Payload(Client &client, std::map<int, Client> &clients, const Request::Body &body) : client(client),
																									   clients(clients),
																									   body(body)
{}

Command::Command(Client &client, std::map<int, Client> &clients) : _client(client), _clients(clients) {}

void Command::ex_cmd(const Request::Body &body)
{
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
			throw AuthException(ERR_NONICKNAMEGIVEN(p.client.get_key("NICKNAME")));
		throw ResponseException(ERR_NONICKNAMEGIVEN(p.client.get_key("NICKNAME")));
	}

	std::string nickname = p.body.second[0];
	if (nickname[0] == ':')
		nickname.erase(0);

	if (nickname.size() > NICKNAME_LENGTH) // todo: check if nickname is valid
	{
		if (!p.client.is_auth())
			throw AuthException(ERR_ERRONEUSNICKNAME(nickname, nickname));
		throw ResponseException(ERR_ERRONEUSNICKNAME(nickname, nickname));
	}

	for (std::map<int, Client>::const_iterator it = p.clients.begin(); it != p.clients.end(); it++)
	{
		if (it->second.get_fd() != -1 && &it->second != &p.client && it->second.get_key("NICKNAME") == nickname)
		{
			if (!p.client.is_auth())
				throw AuthException(ERR_NICKNAMEINUSE(nickname, nickname));
			throw ResponseException(ERR_NICKNAMEINUSE(nickname, nickname));
		}
	}

	p.client["NICKNAME"] = nickname;
	WARNING("Nickname is now: " << p.client["NICKNAME"]);
}

void Command::pass(Payload p)
{
	if (p.client["PASSWORD"].size() > 0)
		throw ResponseException(ERR_ALREADYREGISTRED(p.client["NICKNAME"]));
	if (p.body.second.size() != 1)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client["NICKNAME"], p.body.first));

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

Command::map_t Command::init_cmd()
{
	map_t map;

	map["NICK"] = &Command::nick;
	map["PASS"] = &Command::pass;
	map["USER"] = &Command::user;
	return (map);
}

Command::map_t Command::_commands = Command::init_cmd();
