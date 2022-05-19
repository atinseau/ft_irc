/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:15 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/19 17:50:25 by mbonnet          ###   ########.fr       */
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
			{
				this->AddClient();
			}
			else
			{
					this->protocolReception(i);
					this->sendMessage(i,"MESSAGE coucou\r\n");
			}
		}
	} while (1);
}

//receptionne les message 
void	Server::protocolReception(int i)
{
	int rc;
	do
	{
		std::string line;
		int len = 0;
		rc = this->Reception(&line, &len, i);
		int pars = this->parsing(i, line);
		if (pars == 1)
			this->printReception(i, len, line);
		else if (pars == -1)
		{
			this->closedAndPreventClient(i);
			break ;
		}
		else if (pars == -2)
		{
			std::cout << "ERROR commande channel" << std::endl;
			break ;
		}
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error(ROUGE"recv() failed"BLANC);
			break;
		}
		
	} while(true);
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
	send(this->_pfds[i].fd, "/end", sizeof("/end"), 0);
	close(this->_pfds[i].fd);
	std::cerr << VIOLET << "---SERVER---  Connection" << ROUGE << " closed " << VIOLET << "fd : " << this->_pfds[i].fd << BLANC << std::endl;
	this->_pfds.erase((this->_pfds.begin() + i));
	this->_client.erase((this->_client.begin() + i));
}

void	Server::sendMessage(int i, std::string msg)
{
	int c;
	(void)msg;
	if ((c = send(this->_pfds[i].fd, msg.c_str() , msg.size() , 0)) < 0)
		std::cout << "retour c : " << c << std::endl;
	std::cout << "retour c : " << c << std::endl;
}

int	Server::Reception(std::string *line, int *len, int i)
{
	char buffer[1];
	int rc = 0;

	memset(&buffer, 0, 1);
	while ((rc = recv(this->_pfds[i].fd, buffer, sizeof(char), 0)) >= 0)
	{
		if (buffer[0] == 13 || buffer[0] == 10)
			buffer[0] = '\0';
		(*line) += buffer[0];
		*len += rc; 
		memset(&buffer, 0, 1);
	}
	std::cout << std::endl;
	return (rc);
} 

int	Server::parsing(int i, std::string line)
{
	if (this->_client[i].getIdentify() == false)
		return (this->managementOrdered(i, line));
	else if (this->_client[i].getIdentify() == true)
		return (this->managementOrdered(i, line));
	else if (this->_client[i].getIdentify() == true)
		return (1);
	//else if (this->_client[i].getIdentify() == true && line == "/end")
	//	return (-1);
	return (-1);
}

int		Server::managementOrdered(int i, std::string line)
{
	std::string cmd;
	std::string parametre;
	size_t y = 0;

	for (y = 0; y < line.size(); y++)
		if (whitesapece(line[y]) == true)
			break;
	char tmp[y];
	line.copy(tmp, y ,0);
	tmp[y] = '\0';
	cmd = tmp;
	while (whitesapece(line[y]) == true)
		y++;
	char tmp2[line.size() - y];
	line.copy(tmp2, line.size() - y ,y);
	tmp2[line.size() - y] = '\0';
	parametre = tmp2;
	if (cmd == "PASS" && parametre == this->_password)
		this->_client[i].setIdentify(true);
	else if (cmd == "PASS" && parametre != this->_password)
		return (-1);
	else if (cmd == "USER")
		this->_client[i].setUsername(parametre);
	else if (cmd == "NICK")
		this->_client[i].setNickname(parametre);
	else if (cmd == "CHANNEL")
	{
		for (size_t x = 0; x < parametre.size(); x++)
			if (parametre[x] < 48 || parametre[x] > 57)
				return (-2);
		this->_client[i].setChannel(std::atoi(parametre.c_str()));
	}
	else if (cmd == "MESSAGE")
		return (1);
	return (0);
}


void	Server::printReception(int i, int len, std::string line)
{
	std::cerr << BLEU << "------------------------------------------------"<< BLANC << std::endl;
	std::cerr << BLEU << "RECEPTION"<< BLANC << std::endl;
	std::cerr << BLEU << "Username  : " << this->_client[i].getUsername() << " Nickname  : " << this->_client[i].getNickname() << BLANC << std::endl;
	std::cerr << BLEU << "Channel   : " << this->_client[i].getChannel() << " Fd        : " << this->_pfds[i].fd  << BLANC << std::endl;
	std::cerr << BLEU << "taille    : ("<< len << "bits)"<< BLANC << std::endl;
	std::cerr << BLEU << "message   : "<< line << BLANC << std::endl;
	std::cerr << BLEU << "------------------------------------------------"<< BLANC << std::endl;
}

