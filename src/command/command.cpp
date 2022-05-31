/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:38 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/31 15:16:45 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

using namespace std;

Command::Command(Client& client, std::map<int,Client> &clients): _client(client), _clients(clients) {}

void Command::ex_cmd(const Request::Body& body, std::map<std::string, Channel>& channels)
{
	(void) _client;

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

std::string		Command::return_str_client_channel(std::map<std::string, Channel*> channels, Client &client)
{
	std::string str;
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (client.get_channels().find(it->first) == client.get_channels().end() && 
			(channels.find(it->first)->second->get_mode()['p'] == true || channels.find(it->first)->second->get_mode()['s'] == true))
			continue ;
		str += "\t\tChannel ";
		str += it->first;
		str+= " : ";
		std::map<int, Client*> channel_tmp = it->second->get_clients();
		for (std::map<int, Client*>::iterator it_cli = channel_tmp.begin(); it_cli != channel_tmp.end(); it_cli++)
		{
			if (it_cli->second->get_mode(it->second)['i'] == true)
				continue ;
			str += " " + it_cli->second->get_key("NICKNAME") + " ";
		}
	}
	return (str);
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

	for (std::map<int,Client>::const_iterator it = p.clients.begin(); it != p.clients.end(); it++)
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
	if (p.body.second.size() == 1 && p.body.second.begin()->size() == 1)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	std::vector<std::string> body_channel;
	std::vector<std::string> body_para;
	for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it < p.body.second.end(); it++)
	{
		if ((*it)[0] == '#')
			body_channel.push_back(*it);
		else 
			body_para.push_back(*it);
	}
	for (size_t i = 0; i < body_channel.size(); i++)
	{
		if (p.channels.find(body_channel[i]) != p.channels.end())
		{
			if (p.client.get_channels().find(body_channel[i])->first != p.client.get_channels().end()->first)
				throw ResponseException(ERR_TOOMANYCHANNELS(p.client.get_key("NICKNAME"), p.channels.find(body_channel[i])->first));
			std::map<char,bool> mode = p.channels.find(body_channel[i])->second.get_mode();
			for (std::map<char,bool>::iterator it = mode.begin(); it != mode.end() ; it++)
				p.channels.find(body_channel[i])->second._mode_join[it->first](p.channels.find(body_channel[i])->second, body_channel, body_para, p.client);
			std::map<std::string, Channel*> channel;
			channel.insert(std::pair<std::string, Channel*>(p.body.second[i],&p.channels[p.body.second[i]]));
			p.client.add_channels(std::pair<std::string, Channel*>(p.body.second[i],&p.channels[p.body.second[i]]));
			p.channels[p.body.second[i]].add_client(&p.client);
			p.client.write(ResponseException(RPL_TOPIC(p.client.get_key("NICKNAME"),p.channels.find(body_channel[i])->second.get_topic())).response());
			p.client.write(ResponseException(RPL_NAMREPLY(p.client.get_key("NICKNAME"), return_str_client_channel(channel, p.client))).response());
		}
		else
			throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), p.channels.find(p.body.second[i])->first));
	}
}

void Command::part(Payload p)
{
	if (p.body.second.size() == 1 && p.body.second.begin()->size() == 1)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	std::vector<std::string> body_channel;
	for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it < p.body.second.end(); it++)
		if ((*it)[0] == '#')
			body_channel.push_back(*it);
	for (size_t i = 0; i < body_channel.size(); i++)
	{
		if (p.channels.find(*(body_channel.begin()+ i)) == p.channels.end())
			throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), p.channels.find(*(body_channel.begin()+ i))->first));
		if (p.client.get_channels().find(*(body_channel.begin()+ i)) == p.client.get_channels().end())
			throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), p.channels.find(*(body_channel.begin()+ i))->first));
		p.client.disconnect_channel(p.client.get_channels().find(*(body_channel.begin()+ i))->second , &p.channels);
	}
}


void Command::topic(Payload p)
{
	Channel *channel;
	if (p.body.second.begin() == p.body.second.end() || (*p.body.second.begin())[0] != '#')
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	if (p.client.get_channels().find(*p.body.second.begin()) == p.client.get_channels().end())
		throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), p.client.get_channels().find(*p.body.second.begin())->first));
	channel = p.client.get_channels().find(*p.body.second.begin())->second;
	if (channel->get_mode()['t'] == true && p.client.get_mode(channel)['o'] == false)
		throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), p.client.get_channels().find(*p.body.second.begin())->first));
	if (p.body.second.begin()+1 != p.body.second.end())
		channel->set_topic(*(p.body.second.begin()+1));
	else
	{
		if (channel->get_topic() == "\0")
			p.client.write(ResponseException(RPL_NOTOPIC(p.client.get_key("NICKNAME"))).response());
		else
			p.client.write(ResponseException(RPL_TOPIC(p.client.get_key("NICKNAME"), channel->get_topic())).response());
	}
}

void Command::quit(Payload p)
{
	if (p.body.second.begin() != p.body.second.end())
	{
		std::string message_fin;
		for (std::vector<const std::string>::iterator it = p.body.second.begin(); it != p.body.second.end(); it++)
			message_fin += (*it);
		p.client.write(ResponseException(RPL_MESSAGEEND(p.client.get_key("NICKNAME"), message_fin)).response());
	}
	else
		p.client.write(ResponseException(RPL_MESSAGEEND(p.client.get_key("NICKNAME"), "\0")).response());
	p.client.disconnect(&p.channels);
}

void Command::names(Payload p)
{
	std::map<std::string, Channel *> channel;
	if (*p.body.second.begin() == "\0")
		for (std::map<std::string, Channel>::iterator it_cha = p.channels.begin(); it_cha != p.channels.end(); it_cha++)
			channel.insert(std::pair<std::string, Channel*>(it_cha->first,&(it_cha->second)));
	else
		for (std::vector<const std::string>::iterator it_bod = p.body.second.begin(); it_bod != p.body.second.end(); it_bod++)
			if (p.channels.find(*it_bod) != p.channels.end())
				channel.insert(std::pair<std::string, Channel*>(p.channels.find(*it_bod)->first,&(p.channels.find(*it_bod)->second)));
	p.client.write(ResponseException(RPL_NAMREPLY(p.client.get_key("NICKNAME"), return_str_client_channel(channel, p.client))).response());
	p.client.write(ResponseException(RPL_ENDOFNAMES(p.client.get_key("NICKNAME"))).response());
}

void Command::list(Payload p)
{
	std::string str;
	std::map<std::string, Channel *> channel;
	if (*p.body.second.begin() == "\0")
		for (std::map<std::string, Channel>::iterator it_cha = p.channels.begin(); it_cha != p.channels.end(); it_cha++)
			channel.insert(std::pair<std::string, Channel*>(it_cha->first,&(it_cha->second)));
	else
		for (std::vector<const std::string>::iterator it_bod = p.body.second.begin(); it_bod != p.body.second.end(); it_bod++)
			if (p.channels.find(*it_bod) != p.channels.end())
				channel.insert(std::pair<std::string, Channel*>(p.channels.find(*it_bod)->first,&(p.channels.find(*it_bod)->second)));
	for (std::map<std::string, Channel *>::iterator it = channel.begin(); it != channel.end(); it++)
	{
		if (p.client.get_channels().find(it->first) == p.client.get_channels().end() && 
			(p.channels.find(it->first)->second.get_mode()['s'] == true))
			continue ;
		str += "\t\tChannel ";
		str += it->first;
		if (p.client.get_channels().find(it->first) == p.client.get_channels().end() && 
			(p.channels.find(it->first)->second.get_mode()['p'] == true))
			continue ;
		str += " : " + it->second->get_topic();
	}
	p.client.write(ResponseException(RPL_LISTSTART(p.client.get_key("NICKNAME"))).response());
	p.client.write(ResponseException(RPL_LIST(p.client.get_key("NICKNAME"), str)).response());
	p.client.write(ResponseException(RPL_LISTEND(p.client.get_key("NICKNAME"))).response());
}

void Command::invite(Payload p)
{
	std::map<int,Client>::iterator it = p.clients.begin();
	if (*p.body.second.begin() == "\0")
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	for (; it != p.clients.end(); it++)
		if (it->second.get_key("NICKNAME") == *p.body.second.begin())
			break ;
	if (it == p.clients.end())
		throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"),"l inviter n existe pas"));
	if (p.client.get_channels().find(*(p.body.second.begin() + 1)) != p.client.get_channels().end())
	{
		if (p.channels.find(*(p.body.second.begin() + 1))->second.get_mode()['i'] == true
		&& p.client.get_mode(&p.channels.find(*(p.body.second.begin() + 1))->second)['o'] == false)
			throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), ));
		if (it->second.get_channels().find(*(p.body.second.begin() + 1)) != it->second.get_channels().end())
			throw ResponseException(ERR_USERONCHANNEL(p.client.get_key("NICKNAME"), it->second.get_channels().find(*(p.body.second.begin() + 1))->first));
		it->second.add_channels(std::pair<std::string, Channel*>(p.client.get_channels().find(*(p.body.second.begin() + 1))->first , &p.channels[*(p.body.second.begin() + 1)]));
		p.client.get_channels()[*(p.body.second.begin() + 1)]->add_client(&it->second);
	}
	else if (p.channels.find(*(p.body.second.begin() + 1)) != p.channels.end()) 
		throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), p.channels.find(*(p.body.second.begin() + 1))->first));
	else
	{
		p.channels.insert(std::pair<std::string, Channel>(*(p.body.second.begin() + 1), Channel()));
		p.channels[*(p.body.second.begin() + 1)].add_client(&it->second);
		p.channels[*(p.body.second.begin() + 1)].set_topic(*(p.body.second.begin() + 1));
		it->second.add_channels(std::pair<std::string, Channel*>(*(p.body.second.begin() + 1), &p.channels[*(p.body.second.begin() + 1)]));
	}
	p.client.write(ResponseException(RPL_INVITING(p.client.get_key("NICKNAME"))).response());
	
}

void Command::kick(Payload p)
{
	std::vector<Channel*> channels;
	std::vector<Client*> clients;
	std::vector<std::string>::const_iterator it = p.body.second.begin();
	
	for (; it != p.body.second.end(); it++)
	{
		if ((*it)[0] == '#')
		{
			if (p.channels.find(*it) == p.channels.end())
				throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), *it));
			if (p.client.get_channels().find(*it) == p.client.get_channels().end())
				throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), *it));
			if (p.client.get_mode(&p.channels[*it])['o'] == false)
				throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), *it));
			channels.push_back(&p.channels[*it]);
		}
		else
			break;
	}
	for (; it != p.body.second.end(); it++)
	{
		for (std::map<int,Client>::iterator it_cli = p.clients.begin(); it_cli != p.clients.end(); it_cli++)
			if (it_cli->second.get_key("NICKNAME") == *it)
				clients.push_back(&it_cli->second);
	}
	if (channels.size() != clients.size())
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	for (size_t i = 0; i < clients.size(); i++)
	{
		(*(clients.begin() + i))->disconnect_channel((*(channels.begin() + i)), &p.channels);
		(*(clients.begin() + i))->write(ResponseException(RPL_INVITING((*(clients.begin() + i))->get_key("NICKNAME"))).response());
	}
}

void Command::mode(Payload p)
{
	(void)p;
	
}


Command::map_t Command::init_cmd()
{
	map_t map;

	map["NICK"] = &Command::nick;
	map["PASS"] = &Command::pass;
	map["USER"] = &Command::user;
	map["JOIN"] = &Command::join;
	map["PART"] = &Command::part;
	map["MODE"] = &Command::mode;
	map["TOPIC"] = &Command::topic;
	map["QUIT"] = &Command::quit;
	map["NAMES"] = &Command::names;
	map["LIST"] = &Command::list;
	map["INVITE"] = &Command::invite;
	map["KICK"] = &Command::kick;
	return (map);
}
Command::map_t Command::_commands = Command::init_cmd();

