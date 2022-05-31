/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:18 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/31 10:06:06 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../irc.hpp"

#define BUFFER_SIZE 512

/**
 * @brief 
 * Pas de Kamelcase pour les fonctions !
 * exemple: 
 * 		- get_fd()
 * 		- get_channel()
 * invalide:
 * 		- GetFd()
 * 		- GetChannel()
 */

class Server
{
public:
	Server(std::string port, std::string password);
	~Server(void);
	
	
	void	run(void);

private:
	int					_port;
	int					_sock_server;
	struct sockaddr_in6	_addr_server;

	std::string						_password;
	std::vector<pollfd>				_pfds;
	//std::vector<Client>				_clients;
	std::map<int,Client>			_clients;
	std::map<std::string, Channel>	_channels;



	void			_init();
	void			_new_client(void);
	void			_client_handler(Client& it);
	void			_disconnect(Client& it);
	void			_print_channel();
	std::string		_uuid();
	pollfd			_create_pfd(int fd);
	void			_sup_channel(std::string channel);
};



#endif