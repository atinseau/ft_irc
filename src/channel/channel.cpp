/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:20:08 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/25 07:53:36 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(void) : _topic(""), _password(""), _max_client(-1)
{
	_mode.push_back('k');
}

void	Channel::set_topic(std::string uuid)
{
	this->_topic = uuid;
}

void					Channel::print_clients()
{
	INFO("Client du channel \"" << this->get_topic() << "\"");
	for(std::vector<Client*>::iterator it = this->_clients.begin(); it !=this->_clients.end(); it++)
		INFO("\tclient : " << (*it)->get_key("NICKNAME"));
}

void	Channel::add_client(Client *client)
{
	this->_clients.push_back(client);
	this->print_clients();
}





