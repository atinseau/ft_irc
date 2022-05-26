/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:20:08 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/26 17:19:51 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(void) :  _topic(""), _password(""), _max_client(-1)
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
	if (this->_clients.size() == 0)
		return ;
	for(std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		INFO("\tclient : " << it->second->get_fd());
}

void	Channel::add_client(Client *client)
{
	this->_clients.insert(std::pair<int, Client*>(client->get_fd(),client));
}

size_t		Channel::sup_client(int fd)
{
	if (this->_clients.size() == 0)
		return (0);
	if (this->_clients.find(fd) != this->_clients.end())
		this->_clients.erase(this->_clients.find(fd));
	return (this->_clients.size());
}







