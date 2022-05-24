/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:20:08 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 17:32:46 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(void) : _topic(""), _password(""), _mode('k'), _max_client(-1)
{
	(void)_mode;
	(void)_topic;
	(void)_password;
	(void)_black_list;
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





