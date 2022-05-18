/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:53:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 10:29:30 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../irc.hpp"

class Client
{
public:
	Client(void) : _fd(-1), _channel(-1), _identify(false), _username(""), _nickname("") {(void)this->_fd;(void)this->_channel;(void)this->_identify;(void)this->_nickname;(void)this->_username;};
	Client(int fd) : _fd(fd), _channel(-1), _identify(false), _username(""), _nickname("") {};
	~Client(void){};
private:
	int			_fd;
	int			_channel;
	bool		_identify;
	std::string	_username;
	std::string	_nickname;
};

void	msgServer(std::string str);

#endif