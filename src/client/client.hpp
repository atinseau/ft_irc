/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:53:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/19 10:37:43 by mbonnet          ###   ########.fr       */
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
	int				getFd(void){return (this->_fd);};
	int				getChannel(void){return (this->_channel);};
	bool			getIdentify(void){return (this->_identify);};
	std::string		getUsername(void){return (this->_username);};
	std::string		getNickname(void){return (this->_nickname);};
	void			setFd(int fd){this->_fd = fd;};
	void			setChannel(int channel){std::cout << VERT << "Nouveau channel : " << channel << BLANC << std::endl;this->_channel = channel;};
	void			setIdentify(bool iden){if (this->_identify == false) std::cout << VERT << "Moth de passe correcte" << BLANC << std::endl; this->_identify = iden;};
	void			setUsername(std::string username){std::cout << VERT << "Nouveau username : " << username << BLANC << std::endl;this->_username = username;};
	void			setNickname(std::string nickname){std::cout << VERT << "Nouveau nickname : " << nickname << BLANC << std::endl;this->_nickname = nickname;};
	void			printPara(void);

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