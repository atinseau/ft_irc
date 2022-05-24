/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:15 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 19:59:45 by mbonnet          ###   ########.fr       */
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
	for (std::vector<Client>::iterator it = _client.begin(); it != _client.end(); it++)
		it->disconnect();
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

	this->_client.push_back(Client(NULL));
	_create_pfd(this->_sock_server);

	SUCCESS("Le server est lancé sur le port " << this->_port);
}

/**
 * @brief
 *
 */
void Server::run(void)
{
	int rc, current_size;
	do
	{
		if ((rc = poll(&(*this->_pfds.begin()), this->_pfds.size(), TIME)) <= 0)
			throw std::runtime_error("poll() failed/timeout");
		current_size = this->_pfds.size();
		for (int i = 0; i < current_size; i++)
		{
			if (this->_pfds[i].revents == 0)
				continue;
			if (this->_pfds[i].revents != POLLIN)
			{
				_disconnect(i);
				break;
			}
			if (this->_pfds[i].fd == this->_sock_server)
			{
				_new_client();
			}
			else
			{
				try
				{
					_client_handler(i);
				}
				catch (std::runtime_error &e)
				{
					ERROR(e.what());
					_disconnect(i);
					break;
				}
			}	
		}
	} while (1);
}

/**
 * @brief
 * Interception des message provenant des clients
 * @param id index du client dans le tableau de poll et du client
 */
void Server::_client_handler(int id)
{
	do
	{
		Client &client = this->_client[id];
		Request req = client.read();

		if (req.type() != Request::SUCCESS)
			break;

		Request::Body body = req.body();

		Command cmd(client, _client);
		
		try
		{
			cmd.ex_cmd(req.body(), _channels);
		}
		catch (Command::AuthException &e)
		{
			client.write(e.response());
			_disconnect(id);
		}
		catch (Command::ResponseException &e)
		{
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
			break;
		}
		Client client(_create_pfd(fd));
		this->_client.push_back(client);
		std::string tmp = _uu_id();
		this->_channels.insert(std::pair<std::string, Channel>(tmp, Channel()));
		this->_channels[tmp].add_client((this->_client.end()-1).base());
		client.add_channel(&(this->_channels[tmp]));
	} while (fd != -1);
	this->_nb_channel++;
}

/**
 * @brief
 *
 * @param fd descripteur de fichier du client
 * @return pollfd* pour le poll
 */
pollfd *Server::_create_pfd(int fd)
{
	pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	this->_pfds.push_back(pfd);
	return ((this->_pfds.end() - 1).base());
}

/**
 * @brief
 * Gestion de la deconnexion d un client
 * Ferme le fd et suprime l element du tableau
 * puis appelle le _disconnect du client
 * @param i index d'un client dans le tableau
 */
void Server::_disconnect(int i)
{
	_client[i].disconnect();
	_client.erase((this->_client.begin() + i));
	_pfds.erase(this->_pfds.begin() + i);

	SUCCESS("le client " << i << " a été deconnecté");
}

std::string		Server::_uu_id()
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
	std::string ret("#" + std::to_string(tv.tv_sec + tv.tv_usec));
	std::cout << "id channel : " << ret << std::endl;
	return (ret);
}


int Server::_nb_channel = 0;