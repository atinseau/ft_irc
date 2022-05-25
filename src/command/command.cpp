/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:38 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/25 08:30:04 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

using namespace std;

Command::Command(Client& client, std::vector<Client>& clients): _client(client), _clients(clients) {}

void Command::ex_cmd(const Request::Body& body, std::map<std::string, Channel>& channels)
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
		it->second(Payload(_client, _clients, body, channels));
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
		if (it->get_fd() != -1 && it.base() != &p.client && it->get_key("NICKNAME") == nickname)
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
	std::vector<std::string> body_channel;
	std::vector<std::string> body_para;
	for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it < p.body.second.end(); it++)
	{
		if ((*it)[0] == '#')
			body_channel.push_back(*it);
		else 
			body_para.push_back(*it);
	}
	for (size_t i = 0; i < p.body.second.size(); i++)
	{
		if (p.channels.find(body_channel[i]) != p.channels.end())
		{
			for (size_t x = 0; x < p.client.get_channel().size(); x++)
			{
				std::string channel = (*(p.client.get_channel().begin()+x))->get_topic();
				if (p.channels.find(channel) != p.channels.end())
					throw ResponseException(ERR_CHANNELISALRAIDYCONNECTED(p.client.get_key("NICKNAME")));
			}
			std::vector<char> mode = p.channels.find(body_channel[i])->second.get_mode();
			for (size_t y = 0; y < (mode).size() ; y++)
				if (_mode[mode[y]](p.channels.find(body_channel[i])->second, body_para, p.client) != true)
					throw ResponseException(ERR_CHANNELISNOTAVAILABLE(p.client.get_key("NICKNAME")));
			p.client.add_channel(&(p.channels[p.body.second[i]]));
		}
		else
			throw ResponseException(ERR_CHANNELDOESNOTEXIST(p.client.get_key("NICKNAME")));
	}
}


Command::map_t Command::init_cmd()
{
	map_t map;

	map["NICK"] = &Command::nick;
	map["PASS"] = &Command::pass;
	map["USER"] = &Command::user;
	map["JOIN"] = &Command::join;

	return (map);
}
Command::map_t Command::_commands = Command::init_cmd();

