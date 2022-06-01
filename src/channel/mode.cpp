/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:43:14 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/01 17:18:22 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

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
