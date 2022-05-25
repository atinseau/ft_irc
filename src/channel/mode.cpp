/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:43:14 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/25 19:08:30 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

bool Channel::join_o(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//si ce mode est activer, on restrain l acces uniquement au IRCops operateur IRC
	//il faudra donc modifier cette fonction quand on aura mis sa en place 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	return (false);
}
bool Channel::join_p(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal priver 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	return (false);
}
bool Channel::join_s(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal secret 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	return (false);
}
bool Channel::join_i(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//acces uniquement par invitation 
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	return (false);
}
bool Channel::join_t(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal permetant de changer le sujet que par les operateur
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;	return (true);
}
bool Channel::join_n(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//canal empechant tout message provenant d un client exterieur;
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	return (true);
}
bool Channel::join_m(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//canal empechant les client aillant une affectation trop base de pouvoir parler;
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;	
	return (true);
}
bool Channel::join_l(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//cannal possaidant une limite d invitation
	(void)cmd;
	(void)para;
	(void)client;
	size_t i = -1;
	if (channel.get_max_client() == i)
		return (true);
	if (channel.get_max_client() >= channel.get_clients().size())
		return (false);
	return (true);
}
bool Channel::join_b(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	//canal bloquand les personne sur liste rouge
	(void)cmd;
	(void)para;
	for (size_t i = 0; i < channel.get_black_liste().size(); i++)
	{
		std::string nickName = (*(channel.get_black_liste().begin() + i))->get_key("NICKNAME");
		if (nickName == client.get_key("NICKNAME"))
			return (false);
	}
	return (true);
}
bool Channel::join_v(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	(void)channel;
	(void)cmd;
	(void)para;
	(void)client;
	return (true);
}
bool Channel::join_k(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client)
{
	(void)cmd;
	(void)client;
	if (channel.get_password() == "\0")
		return (true);
	for (std::vector<std::string>::iterator it = para.begin(); it != para.end(); it++)
	{
		if (*it == channel.get_password())
			return (true);
	}
	return (false);
}

std::map<char, Channel::func_t1> Channel::init_mode_join()
{
	std::map<char, Channel::func_t1> map;

	map['o'] = &Channel::join_o;
	map['p'] = &Channel::join_p;
	map['s'] = &Channel::join_s;
	map['i'] = &Channel::join_i;
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