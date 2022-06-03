/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:38 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/03 19:47:22 by mbonnet          ###   ########.fr       */
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
			if (it_cli->second->get_opperator(it->second) == true)
				str += " @";
			else
				str += " "; 
			str += " " + it_cli->second->get_key("NICKNAME") + " ";
		}
	}
	return (str);
}

Client *Command::get_client_by_nickname(std::string nickname, std::map<int, Client> &clients)
{
	for (std::map<int,Client>::iterator it_cli = clients.begin(); it_cli != clients.end(); it_cli++)
	{
		if (it_cli->second.get_key("NICKNAME") == nickname)
			return (&it_cli->second);
	}
	return (&clients.end()->second);
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
		else if (it == p.body.second.begin())
			throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
		else 
			body_para.push_back(*it);
	}
	for (size_t i = 0; i < body_channel.size(); i++)
	{
		if (p.channels.find(body_channel[i]) != p.channels.end())
		{
			if (p.client.get_channels().find(body_channel[i])->first != p.client.get_channels().end()->first)
				throw ResponseException(ERR_USERONCHANNEL(p.client.get_key("NICKNAME"), p.channels.find(body_channel[i])->first));
			std::map<char,bool> mode = p.channels.find(body_channel[i])->second.get_mode();
			for (std::map<char,bool>::iterator it = mode.begin(); it != mode.end() ; it++)
				p.channels.find(body_channel[i])->second._mode_join[it->first](p.channels.find(body_channel[i])->second, body_channel, body_para, p.client);
			std::map<std::string, Channel*> channel;
			channel.insert(std::pair<std::string, Channel*>(p.body.second[i],&p.channels[p.body.second[i]]));
			p.client.add_channels(std::pair<std::string, Channel*>(p.body.second[i],&p.channels[p.body.second[i]]), false);
			p.channels[p.body.second[i]].add_client(&p.client);
			p.client.write(ResponseException(RPL_TOPIC(p.client.get_key("NICKNAME"),p.channels.find(body_channel[i])->second.get_topic())).response());
			p.client.write(ResponseException(RPL_NAMREPLY(p.client.get_key("NICKNAME"), return_str_client_channel(channel, p.client))).response());
		}
		else
		{
			p.channels.insert(std::pair<std::string, Channel>(body_channel[i], Channel()));
			p.channels[body_channel[i]].add_client(&p.client);
			p.channels[body_channel[i]].set_topic(body_channel[i]);
			p.client.add_channels(std::pair<std::string, Channel*>(body_channel[i],&p.channels[body_channel[i]]), true);
			p.client.write(ResponseException(RPL_TOPIC(p.client.get_key("NICKNAME"),p.channels.find(body_channel[i])->second.get_topic())).response());
			p.client.write(ResponseException(RPL_NEWCANAL(p.client.get_key("NICKNAME"), body_channel[i] )).response());
		}
	}
}

void Command::part(Payload p)
{
	if (p.body.second.size() == 1 && p.body.second.begin()->size() == 1)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	std::vector<std::string> body_channel;
	for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it != p.body.second.end(); it++)
		if ((*it)[0] == '#')
			body_channel.push_back(*it);
	body_channel.push_back("\0");
	for (size_t i = 0; i < body_channel.size() - 1; i++)
	{
		if (p.channels.find(*(body_channel.begin()+ i)) == p.channels.end())
			throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"),*(body_channel.begin()+ i)));
		if (p.client.get_channels().find(*(body_channel.begin()+ i)) == p.client.get_channels().end())
			throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), *(body_channel.begin()+ i)));
		std::map<int, Client*> tmp = p.channels.find(*(body_channel.begin()+ i))->second.get_clients();
		for (std::map<int, Client*>::iterator it = tmp.begin(); it != tmp.end(); it++)
			it->second->write(ResponseException(RPL_MOUVPART(p.client.get_key("NICKNAME"),*(body_channel.begin()+ i))).response());
		p.client.disconnect_channel(p.client.get_channels().find(*(body_channel.begin()+ i))->second , &p.channels);
	}
}


void Command::topic(Payload p)
{
	Channel *channel;
	if (p.body.second.begin() == p.body.second.end() || (*p.body.second.begin())[0] != '#')
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	if (p.client.get_channels().find(*p.body.second.begin()) == p.client.get_channels().end())
		throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), *p.body.second.begin()));
	channel = p.client.get_channels().find(*p.body.second.begin())->second;
	if (channel->get_mode()['t'] == true && p.client.get_opperator(channel) == false)
		throw ResponseException(ERR_NOPRIVILEGES(p.client.get_key("NICKNAME")));
	if (p.body.second.begin()+1 != p.body.second.end())
	{
		std::string topic = "\0";
		for (std::vector<std::string>::const_iterator it = p.body.second.begin()+1; it != p.body.second.end(); it++)
			topic += " " + *it;
		channel->set_topic(topic);
	}
	else
	{
		if (channel->get_topic() == "\0")
			p.client.write(ResponseException(RPL_NOTOPIC(p.client.get_key("NICKNAME"))).response());
		else
			p.client.write(ResponseException(RPL_TOPIC(p.client.get_key("NICKNAME"), channel->get_topic())).response());
	}
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
	if (*p.body.second.begin() == "\0")
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	if ((*p.body.second.begin())[0] == '#')
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	Client *cli = get_client_by_nickname(*p.body.second.begin(), p.clients);
	if (cli == &p.clients.end()->second)
		throw ResponseException(ERR_NOTHISUSER(*p.body.second.begin()));
	if (p.client.get_channels().find(*(p.body.second.begin() + 1)) != p.client.get_channels().end())
	{
		if (p.client.get_opperator(&p.channels.find(*(p.body.second.begin() + 1))->second) == false)
			throw ResponseException(ERR_NOPRIVILEGES(p.client.get_key("NICKNAME")));
		if (cli->get_channels().find(*(p.body.second.begin() + 1)) != cli->get_channels().end())
			throw ResponseException(ERR_USERONCHANNEL(p.client.get_key("NICKNAME"), cli->get_channels().find(*(p.body.second.begin() + 1))->first));
		cli->add_channels(std::pair<std::string, Channel*>(p.client.get_channels().find(*(p.body.second.begin() + 1))->first , &p.channels[*(p.body.second.begin() + 1)]), false);
		p.client.get_channels()[*(p.body.second.begin() + 1)]->add_client(cli);
	}
	else if (p.channels.find(*(p.body.second.begin() + 1)) != p.channels.end()) 
		throw ResponseException(ERR_NOTONCHANNEL(p.client.get_key("NICKNAME"), *(p.body.second.begin() + 1)));
	else
	{
		p.channels.insert(std::pair<std::string, Channel>(*(p.body.second.begin() + 1), Channel()));
		p.channels[*(p.body.second.begin() + 1)].add_client(cli);
		p.channels[*(p.body.second.begin() + 1)].set_topic(*(p.body.second.begin() + 1));
		cli->add_channels(std::pair<std::string, Channel*>(*(p.body.second.begin() + 1), &p.channels[*(p.body.second.begin() + 1)]), true);
	}
	p.client.write(ResponseException(RPL_INVITING(p.client.get_key("NICKNAME"))).response());
	
}

void Command::kick(Payload p)
{
	std::vector<Channel*> channels;
	std::vector<std::string> name_cha;
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
			if (p.client.get_opperator(&p.channels[*it]) == false)
				throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), *it));
			channels.push_back(&p.channels[*it]);
			name_cha.push_back(p.channels.find(*it)->first);
		}
		else
			break;
	}
	for (; it != p.body.second.end(); it++)
	{
			Client *cli = get_client_by_nickname(*it, p.clients);
			if (cli != &p.clients.end()->second)
				clients.push_back(cli);
			else
				break ;
	}
	if (channels.size() != clients.size() || channels.size() == 0 || clients.size() == 0)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	std::vector<std::string>::iterator it_name = name_cha.begin();
	for (size_t i = 0; i < clients.size(); i++, it_name++)
	{
		std::map<int, Client*> tmp_cli = (*(channels.begin() + i))->get_clients() ;
		for (std::map<int, Client*>::iterator it = tmp_cli.begin(); it != tmp_cli.end(); it++)
			it->second->write(ResponseException(RPL_MOUVKICK(p.client.get_key("NICKNAME"), (*(clients.begin() + i))->get_key("NICKNAME"), *it_name)).response());
		(*(clients.begin() + i))->disconnect_channel((*(channels.begin() + i)), &p.channels);
	}
}

void Command::privmsg(Payload p)
{
	std::vector<Client *>	list_client;
	std::vector<Channel *>	list_channel;
	std::string				msg = "\0";
	for (std::vector<std::string>::const_iterator it = p.body.second.begin(); it != p.body.second.end(); it++)
	{
		if ((*it)[0] == '#')
		{
			if (p.channels.find(*it) == p.channels.end())
				throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), *it));
			list_channel.push_back(&p.channels.find(*it)->second);
		}
		else if (get_client_by_nickname(*it, p.clients) != &p.clients.end()->second)
			list_client.push_back(get_client_by_nickname(*it, p.clients));
		else
			msg += " " + *it;
	}
	if (list_client.size() == 0 && list_channel.size() == 0)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	else if (msg == "\0")
	{
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	}
	for (std::vector<Client *>::iterator it = list_client.begin(); it != list_client.end(); it++)
		(*it)->write(ResponseException(RPL_MSGPRV((*it)->get_key("NICKNAME"),p.client.get_key("NICKNAME"),msg)).response());
	for (std::vector<Channel *>::iterator it = list_channel.begin(); it != list_channel.end(); it++)
		(*it)->send_msg_all_client(&p.client, msg);
}

void Command::mode(Payload p)
{
	Channel *channel;
	char choose;
	std::string mode;
	std::vector<std::string> para;
	std::vector<std::string>::const_iterator tmp = p.body.second.begin();
	size_t nb_para = 0;
	if ((*tmp)[0] != '#')
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	if (p.channels.find(*tmp) == p.channels.end())
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	channel = &p.channels[*tmp];
	if (p.client.get_opperator(channel) == false)
		throw ResponseException(ERR_CHANOPRIVSNEEDED(p.client.get_key("NICKNAME"), channel->get_topic()));
	if (p.client.get_channels().find(*tmp) == p.client.get_channels().end())
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	tmp++;
	if ((*tmp)[0] != '-' && (*tmp)[0] != '+')
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
	choose = (*tmp)[0];
	mode = &((*tmp)[1]);
	tmp++;
	for (; tmp != p.body.second.end(); tmp++)
		para.push_back(*tmp);
	para.push_back("\0");
	for (int i = 0; mode[i] != '\0'; i++)
	{
		if (nb_para > para.size())
			throw ResponseException(ERR_NEEDMOREPARAMS(p.client.get_key("NICKNAME"), p.body.first));
		channel->_mode_mode[mode[i]](channel, choose, mode[i], para[nb_para],&p.client, &p.clients, &p.channels);
		if (mode[i] == 'o' || (choose == '+' && (mode[i] == 'l' || mode[i] == 'k')))
			nb_para++;
	}
	std::map<int, Client*> tmp_cli = channel->get_clients();
	for (std::map<int, Client*>::iterator it = tmp_cli.begin(); it != tmp_cli.end(); it++)
		it->second->write(ResponseException(RPL_MOUVMODE(p.client.get_key("NICKNAME"), p.channels.find(*p.body.second.begin())->first , mode)).response());
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
	map["NAMES"] = &Command::names;
	map["LIST"] = &Command::list;
	map["INVITE"] = &Command::invite;
	map["KICK"] = &Command::kick;
	map["PRIVMSG"] = &Command::privmsg;
	return (map);
}
Command::map_t Command::_commands = Command::init_cmd();

