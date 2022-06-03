/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:43:14 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/03 14:10:02 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"


Client *get_client_by_nickname_2(std::string nickname, std::map<int, Client*> clients)
{
	for (std::map<int,Client*>::iterator it_cli = clients.begin(); it_cli != clients.end(); it_cli++)
		if (it_cli->second->get_key("NICKNAME") == nickname)
			return (it_cli->second);
	return (clients.end()->second);
}

void Channel::join_o(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//si ce mode est activer, on restrain l acces uniquement au IRCops operateur IRC
	//a revoir avec arthur 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	if (channel.get_mode()['o'] == true)
		if (client.get_opperator(&channel) == false)
			throw ResponseException(ERR_BADCHANMASK(client.get_key("NICKNAME")));
		
}
void Channel::join_pi(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal priver secret et canal sous invitation
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	if (channel.get_mode()['p'] == true || channel.get_mode()['s'] == true || channel.get_mode()['i'] == true)
		throw ResponseException(ERR_INVITEONLYCHAN(client.get_key("NICKNAME"),"en attente"));
}
void Channel::join_t(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal permetant de changer le sujet que par les operateur
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
}
void Channel::join_l(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal possaidant une limite d invitation
	(void)cmd;
	(void)para;
	(void)client;
	if (channel.get_mode()['l'] == false)
		return ;
	size_t i = -1;
	if (channel.get_max_client() != i && channel.get_max_client() >= channel.get_clients().size())
		throw ResponseException(ERR_INVITEONLYCHAN(client.get_key("NICKNAME"),"en attente"));
}

void Channel::join_k(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	(void)cmd;
	(void)client;
	if (channel.get_mode()['k'] == false || channel.get_password() == "\0")
		return ;
	for (std::vector<std::string>::iterator it = para.begin(); it != para.end(); it++)
	{
		if (*it == channel.get_password())
			return ;
	}
	throw ResponseException(ERR_BADCHANNELKEY(client.get_key("NICKNAME"), "en attente"));
}

std::map<char, Channel::func_t1> Channel::init_mode_join()
{
	std::map<char, Channel::func_t1> map;

	map['o'] = &Channel::join_o;
	map['p'] = &Channel::join_pi;
	map['i'] = &Channel::join_pi;
	map['t'] = &Channel::join_t;
	map['l'] = &Channel::join_l;
	map['k'] = &Channel::join_k;
	return (map);
}
std::map<char, Channel::func_t1> Channel::_mode_join = Channel::init_mode_join();


void	Channel::set_mode(char choose, char mode)
{
	if (choose == '-')
		this->_mode[mode] = false;
	else 
		this->_mode[mode] = true;
}

void Channel::mode_o(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels)
{
	(void)mode;
	(void)clients;
	(void)channels;
	Client *client_op = get_client_by_nickname_2(para, channel->get_clients());
	if (client_op == channel->get_clients().end()->second)
		throw ResponseException(ERR_CHANOPRIVSNEEDED(client->get_key("NICKNAME"), "en attente"));
	if (choose == '-')
		client_op->set_opperator(channel, false);
	else
		client_op->set_opperator(channel, true);
}
void Channel::mode_pit(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels)
{
	(void)para;
	(void)client;
	(void)clients;
	(void)channels;
	channel->set_mode(choose, mode);
}
void Channel::mode_l(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels)
{
	(void)mode;
	(void)clients;
	(void)channels;
	int nb = 0;
	if (choose == '-')
	{
		channel->set_max_client(-1);
		return ;
	}
	for (int i = 0; para[i] != '\0'; i++)
		if (para[i] < '0' || para[i] > '9')
			throw ResponseException(ERR_CHANOPRIVSNEEDED(client->get_key("NICKNAME"), "en attente"));
	nb = std::atoi(para.c_str());
	channel->set_max_client(nb);
	
}
void Channel::mode_k(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels)
{
	(void)channel;
	(void)choose;
	(void)mode;
	(void)para;
	(void)client;
	(void)clients;
	(void)channels;
	if (choose == '-')
		channel->set_password("\0");
	else 
		channel->set_password(para);
}
std::map<char, Channel::func_t2> Channel::init_mode_mode()
{
	std::map<char, Channel::func_t2> map;

	map['o'] = &Channel::mode_o;
	map['p'] = &Channel::mode_pit;
	map['i'] = &Channel::mode_pit;
	map['t'] = &Channel::mode_pit;
	map['l'] = &Channel::mode_l;
	map['k'] = &Channel::mode_k;
	return (map);
}
std::map<char, Channel::func_t2> Channel::_mode_mode = Channel::init_mode_mode();