/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:15 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 10:55:30 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

//message type de server
void	msgServer(std::string str)
{
	std::cerr << VIOLET << "---SERVER---  " << str << BLANC << std::endl;
}

//initialisation avec un controle du port et du password donner
Server::Server(std::string port, std::string password) : _password(password)
{
	for (size_t i = 0; i < port.size(); i++)
	{
		if (port[i] < 48 || port[i] > 57)
			throw std::runtime_error(ROUGE"Port incorrecte"BLANC);
		if (i > 5)
			throw std::runtime_error(ROUGE"Port incorrecte"BLANC);
		this->_port = atoi(port.c_str());
		if (this->_port > 65536)
			throw std::runtime_error(ROUGE"Port incorrecte"BLANC);
	}
	this->playServer();
}

//close du server avec close de chaque fd
Server::~Server()
{
	for (int i = 0; this->_pfds[i].fd > 0; i++)
		close(this->_pfds[i].fd);
}

//programation du serveur
void Server::playServer(void)
{
	int rc;
	int on = 1;
	pollfd tmp;

	if ((this->_sockServer = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error(ROUGE"socket() failed"BLANC);
	if ((rc = setsockopt(this->_sockServer, SOL_SOCKET,  SO_REUSEADDR,(char *)&on, sizeof(on))) < 0)
		throw std::runtime_error(ROUGE"setsockopt() failed"BLANC);
	if ((rc = ioctl(this->_sockServer, FIONBIO, (char *)&on)) < 0)
		throw std::runtime_error(ROUGE"ioctl() failed"BLANC);
	memset(&this->_addrServer, 0, sizeof(this->_addrServer));
	this->_addrServer.sin6_family      = AF_INET6;
	memcpy(&this->_addrServer.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	this->_addrServer.sin6_port        = htons(this->_port);
	if ((rc = bind(this->_sockServer,(struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer))) < 0)
			throw std::runtime_error(ROUGE"bind() failed"BLANC);
	if ((rc = listen(this->_sockServer, 32)) < 0)
			throw std::runtime_error(ROUGE"listen() failed"BLANC);
	tmp.fd = this->_sockServer;
	tmp.events = POLLIN;
	this->_pfds.push_back(tmp);
	this->_client.push_back(Client());
	msgServer("Server correctement initialiser...");
}

//protocole d ajoue des fd et de reception des message
void Server::run(void)
{
	int rc, current_size;
	do
	{
		msgServer("En attente de poll()...");
		if ((rc = poll(&(*this->_pfds.begin()), this->_pfds.size(), TIME)) <= 0)
			throw std::runtime_error(ROUGE"poll() failed/timeout"BLANC);
		current_size = this->_pfds.size();
		for (int i = 0; i < current_size; i++)
		{
			if(this->_pfds[i].revents == 0)
				continue;
			if(this->_pfds[i].revents != POLLIN)
			{
				std::cout << ROUGE << "Error! revents = " << this->_pfds[i].revents << BLANC << std::endl;
				this->closedAndPreventClient(i);
				break;
			}
			if (this->_pfds[i].fd == this->_sockServer)
				this->AddClient();
			else
				this->Reception(i);
		}
	} while (1);
}

//ajoute les client 
void	Server::AddClient(void)
{
	pollfd tmp;

	msgServer("Listening socket is readable");
	do
	{
		tmp.events = POLLIN;
		if ((tmp.fd = accept(this->_sockServer, NULL, NULL)) < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error(ROUGE"accept() failed"BLANC);
			break;
		}
		std::cerr << VIOLET << "---SERVER---  Connection" << VERT << " open " << VIOLET << "fd : " << tmp.fd << BLANC << std::endl;
		this->_pfds.push_back(tmp);
		this->_client.push_back(tmp.fd);
	} while (tmp.fd != -1);
}

//previent le client de la fermeture, ferme le fd du client et suprime l element du tableau 
void	Server::closedAndPreventClient(int i)
{
	send(this->_pfds[i].fd, "end", sizeof("end"), 0);
	close(this->_pfds[i].fd);
	std::cerr << VIOLET << "---SERVER---  Connection" << ROUGE << " closed " << VIOLET << "fd : " << this->_pfds[i].fd << BLANC << std::endl;
	this->_pfds.erase((this->_pfds.begin() + i));
	//this->_pfds[i].fd = -1;
	//int tmp = this->_pfds.size();
	//for (int i = 0; i < tmp; i++)
	//{
	//	if (this->_pfds[i].fd == -1)
	//	{
	//		for(int j = i; j < tmp - 1; j++)
	//			this->_pfds[j].fd = this->_pfds[j+1].fd;
	//		i--;
	//		tmp--;
	//	}
	//}
}

//receptionne les message 
void	Server::Reception(int i)
{
	char buffer[4];
	int rc = 0;

	do
	{
		std::cout << "coucou" << std::endl;
		std::string line;
		int len = 0;
		while ((rc = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0)) >= 0)
		{
			len += rc;
			line += buffer;
			memset(&buffer, 0, 4);
		}
		if (rc == 0 || line == "end")
		{
			this->closedAndPreventClient(i);
			break;
		}
		else
		{
			std::cerr << BLEU << "------------------------------------------------"<< BLANC << std::endl;
			std::cerr << BLEU << "RECEPTION"<< BLANC << std::endl;
			std::cerr << BLEU << "source fd : (" << this->_pfds[i].fd << ")" << BLANC << std::endl;
			std::cerr << BLEU << "taille    : ("<< len << "bits)"<< BLANC << std::endl;
			std::cerr << BLEU << "message   : ("<< line << ")"<< BLANC << std::endl;
			std::cerr << BLEU << "------------------------------------------------"<< BLANC << std::endl;
		}
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error(ROUGE"recv() failed"BLANC);
			break;
		}
		rc = send(this->_pfds[i].fd, buffer, len, 0);
		if (rc < 0)
			throw std::runtime_error(ROUGE"send() failed"BLANC);
	} while(true);
}
