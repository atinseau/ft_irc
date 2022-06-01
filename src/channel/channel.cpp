/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:20:08 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/01 17:19:12 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(void) :  _topic(""), _password(""), _max_client(-1)
{
	//_mode.push_back('k');
	_mode.insert(std::pair<char,bool>('o',false));
	_mode.insert(std::pair<char,bool>('p',false));
	_mode.insert(std::pair<char,bool>('i',false));
	_mode.insert(std::pair<char,bool>('t',false));
	_mode.insert(std::pair<char,bool>('l',false));
	_mode.insert(std::pair<char,bool>('k',true));
}

void	Channel::set_topic(std::string uuid)
{
	this->_topic = uuid;
}

void					Channel::print_clients(std::string name_channel)
{
	INFO("Client du channel \"" << name_channel << "\" sujet : " << this->get_topic());
	if (this->_clients.size() == 0)
		return ;
	for(std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		INFO("\tclient : " << it->second->get_fd());
		it->second->print_mode_by_channel(this);
	}

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


void			Channel::send_msg_all_client(Client *client, std::string msg)
{
	for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		std::map<int, Client *>::iterator it_cli = this->_clients.begin();
		for (; it_cli != this->_clients.end(); it_cli++)
		{
			if (it_cli->second->get_key("NICKNAME") == client->get_key("NICKNAME"))
				continue ;
		}
		if (it_cli == this->_clients.end()
		&& (this->_mode['p'] == true ||  this->_mode['i'] == true))
			throw ResponseException(ERR_USERSDONTMATCH(client->get_key("NICKNAME"))); 
		it->second->write(ResponseException(RPL_MSGPRV(it->second->get_key("NICKNAME") , client->get_key("NICKNAME"), msg)).response());
	}
}





