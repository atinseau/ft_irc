/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:43:14 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 17:57:21 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

bool Command::mode_o(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//si ce mode est activer, on restrain l acces uniquement au IRCops operateur IRC
	//il faudra donc modifier cette fonction quand on aura mis sa en place 
	(void)channel;
	(void)cmd;
	(void)client;
	return (false);
}
bool Command::mode_p(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//cannal priver 
	(void)channel;
	(void)cmd;
	(void)client;
	return (false);
}
bool Command::mode_s(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//cannal secret 
	(void)channel;
	(void)cmd;
	(void)client;
	return (false);
}
bool Command::mode_i(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//acces uniquement par invitation 
	(void)channel;
	(void)cmd;
	(void)client;
	return (false);
}
bool Command::mode_t(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//cannal permetant de changer le sujet que par les operateur
	(void)channel;
	(void)cmd;
	(void)client;
	return (true);
}
bool Command::mode_n(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//canal empechant tout message provenant d un client exterieur;
	(void)channel;
	(void)cmd;
	(void)client;
	return (true);
}
bool Command::mode_m(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//canal empechant les client aillant une affectation trop base de pouvoir parler;
	(void)channel;
	(void)cmd;
	(void)client;
	return (true);
}
bool Command::mode_l(Channel channel, std::vector<std::string> cmd, Client &client)
{
	//cannal possaidant une limite d invitation
	(void)cmd;
	(void)client;
	if (channel.get_max_client() >= channel.get_clients().size())
		return (false);
	return (true);
}
bool Command::mode_b(Channel channel, std::vector<std::string> cmd, Client &client)
{
	(void)cmd;
	(void)client;
	//canal bloquand les personne sur liste rouge
	for (size_t i = 0; i < channel.get_black_liste().size(); i++)
	{
		std::string nickName = (*(channel.get_black_liste().begin() + i))->get_key("NICKNAME");
		if (nickName == client.get_key("NICKNAME"))
			return (false);
	}
	return (true);
}
bool Command::mode_v(Channel channel, std::vector<std::string> cmd, Client &client)
{
	(void)client;
	(void)channel;
	(void)cmd;
	return (true);
}
bool Command::mode_k(Channel channel, std::vector<std::string> cmd, Client &client)
{
	(void)client;
	if (channel.get_password() == "\0")
		return (true);
	for (std::vector<std::string>::iterator it = cmd.begin(); it != cmd.end(); it++)
		if (*it == channel.get_password())
			return (true);
	return (false);
}

std::map<char, Command::func_t2> Command::init_mode()
{
	std::map<char, Command::func_t2> map;

	map['o'] = &Command::mode_o;
	map['p'] = &Command::mode_p;
	map['s'] = &Command::mode_s;
	map['i'] = &Command::mode_i;
	map['t'] = &Command::mode_t;
	map['n'] = &Command::mode_n;
	map['m'] = &Command::mode_m;
	map['l'] = &Command::mode_l;
	map['b'] = &Command::mode_b;
	map['v'] = &Command::mode_v;
	map['k'] = &Command::mode_k;

	return (map);
}
std::map<char, Command::func_t2> Command::_mode = Command::init_mode();