/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:20:08 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/25 19:05:07 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(void) : _topic(""), _password(""), _max_client(-1)
{
	//_mode.push_back('k');
	_mode.insert(std::pair<char,bool>('o',false));
	_mode.insert(std::pair<char,bool>('p',false));
	_mode.insert(std::pair<char,bool>('s',false));
	_mode.insert(std::pair<char,bool>('i',false));
	_mode.insert(std::pair<char,bool>('t',false));
	_mode.insert(std::pair<char,bool>('n',false));
	_mode.insert(std::pair<char,bool>('m',false));
	_mode.insert(std::pair<char,bool>('l',false));
	_mode.insert(std::pair<char,bool>('b',false));
	_mode.insert(std::pair<char,bool>('v',false));
	_mode.insert(std::pair<char,bool>('k',true));
}

void	Channel::set_topic(std::string uuid)
{
	this->_topic = uuid;
}

void					Channel::print_clients()
{
	INFO("Client du channel \"" << this->get_topic() << "\"");
	for(std::vector<Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		INFO("\tclient : " << (*it)->get_key("NICKNAME"));
}

void	Channel::add_client(Client *client)
{
	this->_clients.push_back(client);
}






