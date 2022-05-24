/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:15 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 10:47:55 by mbonnet          ###   ########.fr       */
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
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
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
			_new_client();

		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			pollfd *pfd = it->get_pfd();

			if (pfd->revents & POLLIN)
			{
				try
				{
					_client_handler(*it);
				}
				catch (std::runtime_error &e)
				{
					ERROR(e.what());
					_disconnect(it);
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
void Server::_client_handler(Client &client)
{

	do
	{
		Request req = client.read();

		if (req.type() != Request::SUCCESS)
			break;

		Request::Body body = req.body();

		DEBUG("Commande: " << body.first);
		DEBUG("Argument: " << body.second);

		// Command cmd(client, _client);

		// try
		// {
		// 	cmd.ex_cmd(req.body(), _channels);
		// }
		// catch (Command::AuthException &e)
		// {
		// 	client.write(e.response());
		// 	_disconnect(id);
		// }
		// catch (Command::ResponseException &e)
		// {
		// 	client.write(e.response());
		// }
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
	// do
	// {
		if ((fd = accept(this->_sock_server, NULL, NULL)) < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("accept() failed");
			return;
		}
		Client client(_create_pfd(fd));
		this->_clients.push_back(client);

		// std::string tmp = _uuid();
		// this->_channels.insert(std::pair<std::string, Channel>(tmp, Channel()));
		// this->_channels[tmp].add_client((this->_clients.end() - 1).base());
		// client.add_channel(&(this->_channels[tmp]));

	// } while (fd != -1);
	// this->_nb_channel++;
}

/**
 * @brief
 *
 * @param fd descripteur de fichier du client
 * @return pollfd pour le poll
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
void Server::_disconnect(std::vector<Client>::iterator &it)
{
	it->disconnect();
	SUCCESS("le client " << it->get_fd() << " a été deconnecté");
	for (std::vector<pollfd>::iterator et = this->_pfds.begin(); et != this->_pfds.end(); et++)
	{
		if (et.base() == it->get_pfd())
		{
			this->_pfds.erase(et);
			break;
		}
	}
	_clients.erase(it);
}

std::string Server::_uuid()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::string ret("#" + std::to_string(tv.tv_sec + tv.tv_usec));
	std::cout << "id channel : " << ret << std::endl;
	return (ret);
}

int Server::_nb_channel = 0;