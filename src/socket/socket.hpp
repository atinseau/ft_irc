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


class Server
{
public:
	Server(std::string port, std::string password);
	~Server(void);


	void	run(void);

	static std::map<int, Client>	clients;
	static std::map<std::string, Channel> channels;
	static std::string				password;
	static std::string 				name;
	static struct timeval 			launch_time;

	static int						port;
	static struct sockaddr_in6		address;
	static bool						running;

private:
	
	int								_sock_server;
	std::vector<pollfd>				_pfds;

	void			_init();
	void			_new_client(void);
	void			_client_handler(std::map<int, Client>::iterator& it);
	void			_disconnect(std::map<int, Client>::iterator& it);
	pollfd			_create_pfd(int fd);
};



#endif