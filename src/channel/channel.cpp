/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:20:08 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 17:56:19 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::Channel(void) : _topic(""), _password(""), _mode('\0')
{
	(void)_mode;
	(void)_topic;
	(void)_password;
	(void)_black_list;
}

void	Channel::add_client(Client *client)
{
	this->_clients.push_back(client);
}


