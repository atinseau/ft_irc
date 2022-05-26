/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:43:14 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/26 08:34:30 by mbonnet          ###   ########.fr       */
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
		if (client.get_mode()['o'] == false)
			return ;
			throw ResponseException(ERR_BADCHANMASK(client.get_key("NICKNAME")));
		
}
void Channel::join_psi(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal priver 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	if (channel.get_mode()['p'] == true || channel.get_mode()['s'] == true || channel.get_mode()['i'] == true)
			return ;
		//throw ResponseException(ERR_INVITEONLYCHAN(client.get_key("NICKNAME")));
}
void Channel::join_t(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal permetant de changer le sujet que par les operateur
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
}
void Channel::join_n(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//canal empechant tout message provenant d un client exterieur;
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
}
void Channel::join_m(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//canal empechant les client aillant une affectation trop base de pouvoir parler;
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
			return ;
		
		//throw ResponseException(ERR_INVITEONLYCHAN(client.get_key("NICKNAME")));
}
void Channel::join_b(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//canal bloquand les personne sur liste rouge
	(void)cmd;
	(void)para;
	if (channel.get_mode()['b'] == false)
		return ;
	for (size_t i = 0; i < channel.get_black_liste().size(); i++)
	{
		std::string nickName = (*(channel.get_black_liste().begin() + i))->get_key("NICKNAME");
		if (nickName == client.get_key("NICKNAME"))
			return ;
			
			//throw ResponseException(ERR_BANNEDFROMCHAN(client.get_key("NICKNAME")));
	}
}
void Channel::join_v(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//le ne comprend pas bien sont utiliter, a revoir 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	if (channel.get_mode()['b'] == false)
		return ;
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
	//throw ResponseException(ERR_BANNEDFROMCHAN(client.get_key("NICKNAME")));
}

std::map<char, Channel::func_t1> Channel::init_mode_join()
{
	std::map<char, Channel::func_t1> map;

	map['o'] = &Channel::join_o;
	map['p'] = &Channel::join_psi;
	map['s'] = &Channel::join_psi;
	map['i'] = &Channel::join_psi;
	map['t'] = &Channel::join_t;
	map['n'] = &Channel::join_n;
	map['m'] = &Channel::join_m;
	map['l'] = &Channel::join_l;
	map['b'] = &Channel::join_b;
	map['v'] = &Channel::join_v;
	map['k'] = &Channel::join_k;

	return (map);
}
std::map<char, Channel::func_t1> Channel::_mode_join = Channel::init_mode_join();