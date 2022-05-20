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

Client::Client(pollfd *pfd): _pfd(pfd), _auth(false)
{
	if (!_pfd)
		return;
	
	INFO("Nouveau client " << _pfd->fd);

	_data["USERNAME"] = "";
	_data["NICKNAME"] = "";
	_data["CHANNEL"] = "";
	_data["PASSWORD"] = "";
}

Client::Request Client::read()
{
	Request req;

	char buffer[BUFFER_SIZE];
	req.first = recv(this->get_fd(), buffer, BUFFER_SIZE, 0);
	if (req.size() < 0)
	{
		if (errno != EWOULDBLOCK)
			req.set_type(Request::ERROR);
		return (req);
	}

	if (req.size() == 0)
	{
		req.set_type(Request::QUIT);
		return (req);
	}
	
	buffer[req.size()] = '\0';
	req.second = buffer;
	req.set_type(Request::SUCCESS);
	return (req);
}

void Client::disconnect()
{
	if (!_pfd)
		return;
	close(_pfd->fd);
}

int Client::get_fd() const
{
	return (_pfd->fd);
}

bool	Client::check_auth(std::string pass)
{
	if (_data["PASSWORD"] == pass && _data["USERNAME"] != ""
		&& _data["NICKNAME"] != "")
	{
		_auth = true;
		return (true);
	}
	return false;
}


