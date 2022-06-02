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

Client::Client(pollfd pfd): _pfd(pfd)
{
	SUCCESS("Nouveau client " << _pfd.fd);

	_data["USERNAME"] = "";
	_data["REALNAME"] = "";
	_data["NICKNAME"] = "";
	_data["PASSWORD"] = "";
	add_mode(_channels.begin()->second, true);
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

void Client::disconnect(std::map<std::string, Channel> *channels)
{
	while (this->_channels.begin() != this->_channels.end())
	{
		std::string id_channel = this->get_channels().begin()->first;
		Channel *channel = this->get_channels().begin()->second;
		this->disconnect_channel(channel , channels);
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

std::string& Client::operator[](const char* key)
{
	return (_data[key]);
}

std::string Client::get_key(const char* key) const
{
	return (_data.at(key));
}

std::map<std::string, Channel*>&	Client::get_channels()
{
	return (this->_channels);
}

bool			Client::get_opperator(Channel* channel)
{
	return (this->_opperator[channel]);
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

void Client::print_channel()
{
	INFO("liste channel du client \"" << this->get_key("NICKNAME") << "\" : ");
	for(std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		INFO("\tchannel : " << it->first);
}

void Client::print_mode_by_channel(Channel *channel)
{
	if (_opperator[channel] == true)
		INFO("\t\tmode : " << "true");
	else if (_opperator[channel] == false)
		INFO("\t\tmode : " << "false");

}

void Client::add_channels(std::pair<std::string , Channel*> channel, bool choix)
{
	_channels.insert(channel);
	add_mode(channel.second, choix);
}

void		Client::disconnect_channel(Channel *channel, std::map<std::string, Channel> *all_channels)
{
	int res;
	if (all_channels->size() == 0)
		return ;
	res = channel->sup_client(this->_pfd.fd);
	if (this->_channels.size() != 0 &&  this->_channels.find(channel->get_topic()) != this->_channels.end())
	{
		this->_channels.erase(this->_channels.find(channel->get_topic()));
		this->_opperator.erase(this->_opperator.find(channel));
	}
	if (all_channels->size() > 0 && res == 0)
		all_channels->erase(all_channels->find(channel->get_topic()));
}

void		Client::add_mode(Channel *channel, bool choi)
{
	_opperator.insert(std::pair<Channel*, bool>(channel, choi));
}

void		Client::set_opperator(Channel *channel, bool choose)
{
	if (_opperator.find(channel) != _opperator.end())
		_opperator.find(channel)->second = choose;
}
