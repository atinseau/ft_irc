/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:15 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/26 17:20:35 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

/**
 * @brief
 * Constructeur de la class Server
 * va uniquement verifier que le port est correct
 * et va appeler la fonction _init) juste après
 *
 * @param port
 * @param password
 */
Server::Server(std::string port, std::string password) : _password(password)
{
	for (size_t i = 0; i < port.size(); i++)
	{
		if (port[i] < 48 || port[i] > 57)
			throw std::runtime_error("Port incorrecte");
		if (i > 5)
			throw std::runtime_error("Port incorrecte");
		this->_port = atoi(port.c_str());
		if (this->_port > 65536)
			throw std::runtime_error("Port incorrecte");
	}
	Client::server_password = this->_password;
	_init();
}

/**
 * @brief
 * Destructeur de la class Server
 * va fermer tout les clients encore connectés
 */
Server::~Server()
{
	for (std::map<int,Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		it->second.disconnect(&this->_channels);
	close(this->_sock_server);
}

/**
 * @brief
 * Crée un socket et l'attache au port donner
 * utilisation de l'ipv6
 * puis création d'un "client" tampon pour faire matcher les id des futurs clients
 */
void Server::_init(void)
{
	int rc;
	int on = 1;

	if ((this->_sock_server = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("socket() failed");
	if ((rc = setsockopt(this->_sock_server, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0)
		throw std::runtime_error("setsockopt() failed");
	if ((rc = ioctl(this->_sock_server, FIONBIO, (char *)&on)) < 0)
		throw std::runtime_error("ioctl() failed");

	memset(&this->_addr_server, 0, sizeof(this->_addr_server));
	this->_addr_server.sin6_family = AF_INET6;
	memcpy(&this->_addr_server.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	this->_addr_server.sin6_port = htons(this->_port);

	if ((rc = bind(this->_sock_server, (struct sockaddr *)&this->_addr_server, sizeof(this->_addr_server))) < 0)
		throw std::runtime_error("bind() failed");
	if ((rc = listen(this->_sock_server, 32)) < 0)
		throw std::runtime_error("listen() failed");

	_create_pfd(this->_sock_server);

	SUCCESS("Le server est lancé sur le port " << this->_port);
}

/**
 * @brief
 *
 */
void Server::run(void)
{
	int rc;
	do
	{
		if ((rc = poll(this->_pfds.begin().base(), this->_pfds.size(), TIME)) <= 0)
			throw std::runtime_error("poll() failed/timeout");

		if (this->_pfds[0].revents & POLLIN)
		{
			_new_client();

		}

		for (std::map<int,Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			try
			{
				_client_handler(it->second);
			}
			catch (std::runtime_error &e)
			{
				ERROR(e.what());
				_disconnect(it->second);
				break;
			}
		}
	} while (1);
}

/**
 * @brief
 * Interception des message provenant des clients
 * @param id index du client dans le tableau de poll et du client
 */
void Server::_client_handler(Client& it)
{
	Client &client = it;
	do
	{
		Request req = client.read();
		if (req.type() != Request::SUCCESS)
		{
			if (req.type() != Request::NONE)
				throw std::runtime_error("Deconnexion");
			break;
		}

		Request::Body body = req.body();
		Command cmd(client, _clients);
		try
		{
			cmd.ex_cmd(req.body(), _channels);
			// un if temporaire a retirer plus tard
		}
		catch (Command::AuthException &e)
		{
			client.write(e.response());
			_disconnect(it);
		}
		catch (Command::ResponseException &e)
		{
			if (req.body().first == "JOIN")
				_print_channel();
			WARNING("Commande non executer");
			client.write(e.response());
		}
	} while (true);
}

/**
 * @brief
 * Attend un nouveau client et l'ajoute au tableau de client
 * crée un nouveau pollfd pour le client via _create_pfd
 * ! il faut check si les fd != -1 est nécessaire
 */
void Server::_new_client(void)
{
	int fd;
	do
	{
		if ((fd = accept(this->_sock_server, NULL, NULL)) < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("accept() failed");
			return;
		}
		Client client(_create_pfd(fd));
		this->_clients.insert(std::pair<int,Client>(fd,client));

		std::string tmp = _uuid();
		this->_channels.insert(std::pair<std::string, Channel>(tmp, Channel()));
		this->_channels[tmp].add_client(&this->_clients.find(fd)->second);
		this->_channels[tmp].set_topic(tmp);
		this->_clients.find(fd)->second.add_channels(std::pair<std::string, Channel*>(tmp,&this->_channels[tmp]));
	} while (fd != -1);
}

/**
 * @brief
 *
 * @param fd descripteur de fichier du client
 * @return pollfd pour le poll
 */
pollfd Server::_create_pfd(int fd)
{
	pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	this->_pfds.push_back(pfd);
	return (pfd);
}

/**
 * @brief
 * Gestion de la deconnexion d un client
 * Ferme le fd et suprime l element du tableau
 * puis appelle le _disconnect du client
 * @param i index d'un client dans le tableau
 */
void Server::_disconnect(Client& it)
{
	
	it.disconnect(&this->_channels);
	SUCCESS("le client " << it.get_fd() << " a été deconnecté");
	for (std::vector<pollfd>::iterator et = this->_pfds.begin(); et != this->_pfds.end(); et++)
	{
		if (et->fd == it.get_fd())
		{
			this->_pfds.erase(et);
			break;
		}
	}
	_clients.erase(_clients.find(it.get_fd()));
}

std::string Server::_uuid()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::string ret("#" + std::to_string(tv.tv_sec + tv.tv_usec));
	return (ret);
}

void			Server::_print_channel()
{
	for(std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		this->_channels[it->first].print_clients();
	}
}
