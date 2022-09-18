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

Client::Client(pollfd pfd) : _pfd(pfd), _state(IS_NOT_AUTH)
{
	WARNING(pfd.fd << " - Nouveau client crée, en attente d'authentification....");

	struct timeval tv;
	gettimeofday(&tv, NULL);

	_data["USERNAME"] = "";
	_data["REALNAME"] = "";
	_data["NICKNAME"] = "";
	_data["PASSWORD"] = "";
	_data["HOSTNAME"] = HOST;
	_data["TIME"] = utils::itoa(tv.tv_sec);
}

Client::~Client() {}

Request Client::read()
{
	Request req;

	char buffer[BUFFER_SIZE] = {0};

	static std::string chunk = "";

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

	if (chunk[chunk.size() - 1] == '\n')
	{
		while(isspace(chunk[chunk.size() - 1]))
			chunk.erase(chunk.size() - 1);
		req.second = chunk;
		if (DEV) DEBUG(req.second);
		req.set_ready(true);
		chunk.clear();
	}
	req.set_type(Request::SUCCESS);
	return (req);
}

void Client::write(Response res)
{
	if (DEV) WARNING(res.str().substr(0, res.str().size() - 1));
	if (send(this->get_fd(), res.str().c_str(), res.str().size(), 0) < 0)
		ERROR("Erreur lors de l'envoi de la requête");
}

void Client::disconnect()
{
	for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		leave(*it);
		std::map<std::string, Channel>::iterator channel_it = Server::channels.find(*it);
		if (channel_it == Server::channels.end() || channel_it->second.connected_clients() > 0)
			continue;
		Server::channels.erase(channel_it);
	}
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

std::string& Client::operator[](const char *key)
{
	return (_data[key]);
}

const std::string& Client::get_key(const char *key) const
{
	return (_data.at(key));
}

void Client::join(std::string channel)
{
	_channels.push_back(channel);
}


void Client::remove_channel(const std::string& channel)
{
	std::vector<std::string>::iterator it = utils::find(_channels.begin(), _channels.end(), channel);
	if (it != _channels.end())
		_channels.erase(it);
}


bool Client::leave(std::string channel)
{
	std::map<std::string, Channel>::iterator channel_it = Server::channels.find(channel);
	if (channel_it != Server::channels.end())
	{
		if (channel_it->second.include(*this))
		{
			Channel::Dispatcher dispatcher = channel_it->second.create_dispatcher();

			dispatcher.load(RPL_PART(fullname(), channel_it->first));
			dispatcher.send();
			
			channel_it->second.kick(*this);
			return (true);
		}
	}
	return (false);
}

bool Client::is_auth()
{
	if (
		_data["USERNAME"] == "" ||
		_data["NICKNAME"] == "" ||
		_data["PASSWORD"] == "")
		return (false);

	if (Server::password != _data["PASSWORD"])
		return (false);
	return (true);
}

void Client::set_state(ClientState state)
{
	this->_state = state;
}

ClientState Client::get_state() const
{
	return (this->_state);
}

std::string Client::fullname() const
{
	return get_key("NICKNAME") + "!" + get_key("USERNAME") + "@" + get_key("REALNAME");
}

std::vector<std::string> Client::get_info(bool print) const
{
	std::vector<std::string> infos;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int diff_time = tv.tv_sec - utils::atoi(get_key("TIME").c_str());
	infos.push_back("Surnom du client: " + get_key("NICKNAME"));
	infos.push_back("Nom d'utilisateur du client: " + get_key("USERNAME"));
	infos.push_back("Connecté depuis: " + utils::itoa(diff_time) + " secondes");
	if (print)
	{
		for (std::vector<std::string>::iterator it = infos.begin(); it != infos.end(); it++)
			DEBUG(*it);
	}
	return !print ? infos : std::vector<std::string>();
}


std::vector<Channel*> Client::get_channels() const
{
	std::vector<Channel*> channels;
	for (std::vector<std::string>::const_iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		std::map<std::string, Channel>::iterator channel_it = Server::channels.find(*it);
		if (channel_it != Server::channels.end())
			channels.push_back(&channel_it->second);
	}
	return channels;
}