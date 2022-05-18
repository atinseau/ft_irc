/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:18 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 10:55:48 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../irc.hpp"

class Server
{
public:
	Server(std::string port, std::string password);
	~Server(void);
	void	playServer();					//programation du serveur
	void	run(void);						//protocole d ajoue des fd et de reception des message
	void	Reception(int i);				//receptionne les message
	void	closedAndPreventClient(int i);	//previent le client de la fermeture, ferme le fd du client et suprime l element du tableau 
	void	AddClient(void);				//ajoute les client 
private:
	int					_port;
	std::string			_password;
	int					_sockServer;
	struct sockaddr_in6	_addrServer;
	std::vector<pollfd>	_pfds;
	std::vector<Client>	_client;
	Server(void){};
};

void	msgServer(std::string str);

#endif