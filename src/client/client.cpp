/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:09 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/19 09:16:08 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

Client::Client(pollfd *pfd): _pfd(pfd)
{
	if (!_pfd)
		throw std::runtime_error("Le pointeur sur le pollfd est null");
	INFO("Nouveau client " << _pfd->fd);
}

Client::Request Client::read()
{
	Request req;

	char buffer[BUFFER_SIZE];
	req.first = recv(fd(), buffer, BUFFER_SIZE, 0);
	if (req.size() <= 0)
		return (req);
	buffer[req.size()] = '\0';
	
	req.second = buffer;

	return (req);
}

void Client::disconnect()
{
	close(_pfd->fd);
}

int Client::fd() const
{
	return (_pfd->fd);
}

