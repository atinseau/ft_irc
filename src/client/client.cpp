/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:09 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 15:24:59 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

std::string Client::server_password = "";

Client::Client(pollfd pfd) : _pfd(pfd)
{
	SUCCESS("Nouveau client " << _pfd.fd);

	_data["USERNAME"] = "";
	_data["REALNAME"] = "";
	_data["NICKNAME"] = "";
	_data["PASSWORD"] = "";
}

Client::~Client() 
{
	
}

Request Client::read()
{
	Request req;

	char buffer[BUFFER_SIZE];

	static std::string chunk;

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

	chunk += buffer;
	req.second = chunk;

	if (chunk.back() == '\n')
	{
		chunk.clear();
		req.set_ready(true);
		req.second.pop_back();
		if (isspace(req.second.back()))
			req.second.pop_back();
	}

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
	close(_pfd.fd);
}

int Client::get_fd() const
{
	return (_pfd.fd);
}

pollfd Client::get_pfd()
{
	return (_pfd);
}

std::string &Client::operator[](const char *key)
{
	return (_data[key]);
}

std::string Client::get_key(const char *key) const
{
	return (_data.at(key));
}


bool Client::is_auth()
{
	if (
		_data["USERNAME"] == "" ||
		_data["NICKNAME"] == "" ||
		_data["PASSWORD"] == "")
		return (false);

	if (Client::server_password != _data["PASSWORD"])
		return (false);
	return (true);
}

