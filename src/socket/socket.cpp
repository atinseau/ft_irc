#include "socket.hpp"

std::map<int, Client> Server::clients = std::map<int, Client>();
std::map<std::string, Channel> Server::channels = std::map<std::string, Channel>();
std::string Server::password = "";
std::string Server::name = "ft_irc";
int Server::port = 0;
struct sockaddr_in6 Server::address = {};
struct timeval Server::launch_time = {};

Server::Server(std::string port, std::string password)
{
	for (size_t i = 0; i < port.size(); i++)
	{
		if (port[i] < 48 || port[i] > 57)
			throw std::runtime_error("Port incorrecte");
		if (i > 5)
			throw std::runtime_error("Port incorrecte");
		Server::port = atoi(port.c_str());
		if (Server::port > 65536)
			throw std::runtime_error("Port incorrecte");
	}
	Server::password = password;
	gettimeofday(&Server::launch_time, NULL);
	_init();
}

Server::~Server()
{
	for (std::map<int, Client>::iterator it = Server::clients.begin(); it != Server::clients.end(); it++)
		it->second.disconnect();
	close(this->_sock_server);
}

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

	Server::address.sin6_family = AF_INET6;
	Server::address.sin6_port = htons(Server::port);

	if ((rc = bind(this->_sock_server, (struct sockaddr *)&Server::address, sizeof(Server::address))) < 0)
		throw std::runtime_error("bind() failed");
	if ((rc = listen(this->_sock_server, 32)) < 0)
		throw std::runtime_error("listen() failed");

	_create_pfd(this->_sock_server);

	SUCCESS("Le server est lancé sur le port " << Server::port);
}

void Server::run(void)
{
	int rc;
	do
	{
		if ((rc = poll(&this->_pfds[0], this->_pfds.size(), TIME)) <= 0)
			throw std::runtime_error("poll() failed/timeout");

		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents == 0)
				continue;

			if (_pfds[i].revents & POLLIN)
			{
				if (_pfds[i].fd == _sock_server)
				{
					_new_client();
					continue;
				}
				std::map<int, Client>::iterator it = Server::clients.find(_pfds[i].fd);
				if (it == Server::clients.end())
					throw std::runtime_error("Client inconnu");
				try
				{
					_client_handler(it);
				}
				catch (std::exception &e)
				{
					_disconnect(it);
				}
			}
		}
	} while (true);
}

void Server::_client_handler(std::map<int, Client>::iterator &it)
{
	Client &client = it->second;

	Request req = client.read();

	if (req.type() != Request::SUCCESS)
	{
		if (req.type() != Request::NONE)
			throw std::runtime_error("Deconnexion");
		return;
	}
	Command cmd(client);
	cmd.exec(req.body());
}

void Server::_new_client(void)
{
	int fd;

	if ((fd = accept(this->_sock_server, NULL, NULL)) < 0)
	{
		if (errno != EWOULDBLOCK)
			throw std::runtime_error("accept() failed");
		return;
	}
	Client client(_create_pfd(fd));
	Server::clients.insert(std::pair<int, Client>(fd, client));
}

pollfd Server::_create_pfd(int fd)
{
	pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	this->_pfds.push_back(pfd);
	return (pfd);
}

void Server::_disconnect(std::map<int, Client>::iterator &it)
{
	it->second.disconnect();
	for (std::vector<pollfd>::iterator et = this->_pfds.begin(); et != this->_pfds.end(); et++)
	{
		if (et->fd == it->second.get_fd())
		{
			this->_pfds.erase(et);
			break;
		}
	}
	INFO("le client " << it->second.get_fd() << " a été deconnecté");
	Server::clients.erase(it);
}
