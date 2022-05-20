/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:18 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/19 09:17:34 by mbonnet          ###   ########.fr       */
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

	std::string			_password;
	std::vector<pollfd>	_pfds;
	std::vector<Client>	_client;

	void	_init();
	void	_new_client(void);
	void	_client_handler(int id);
	void	_disconnect(int i);

	pollfd*	_create_pfd(int fd);
};



#endif