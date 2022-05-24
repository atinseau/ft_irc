/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:09 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 10:48:16 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

std::string Client::server_password = "";

Client::Client(pollfd *pfd): _pfd(pfd)
{
	if (!_pfd)
		return;
	
	INFO("Nouveau client " << _pfd->fd);

	_data["USERNAME"] = "";
	_data["REALNAME"] = "";
	_data["NICKNAME"] = "";
	_data["PASSWORD"] = "";
}


Request Client::read()
{
	Request req;

	char buffer[BUFFER_SIZE];

	req.first = recv(this->get_fd(), buffer, BUFFER_SIZE, 0);

	if (req.first < 0)
	{
		if (errno != EWOULDBLOCK)
			req.set_type(Request::ERROR);
		return (req);
	}
	if (req.first == 0)
	{
		req.set_type(Request::QUIT);
		return (req);
	}
	
	buffer[req.first] = '\0';
	req.second = buffer;
	req.set_type(Request::SUCCESS);
	return (req);
}

void Client::write(Response res)
{
	if (send(this->get_fd(), res.str().c_str(), res.str().size(), 0) < 0)
		ERROR("Erreur lors de l'envoi de la requête");
}

void Client::disconnect()
{
	if (!_pfd)
		return;
	close(_pfd->fd);
}

int Client::get_fd() const
{
	if (!_pfd)
		return (-1);
	return (_pfd->fd);
}

pollfd* Client::get_pfd()
{
	return (_pfd);
}

std::string& Client::operator[](const char* key)
{
	return (_data[key]);
}

std::string Client::get_key(const char* key) const
{
	return (_data.at(key));
}

bool	Client::is_auth()
{
	if (
		_data["USERNAME"] == "" ||
		_data["NICKNAME"] == "" ||
		_data["PASSWORD"] == ""
	)
		return (false);

	if (Client::server_password != _data["PASSWORD"])
		return (false);
	return (true);
}


void Client::add_channel(Channel* channel)
{
	_channels.push_back(channel);
};
