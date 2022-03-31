#include "socket.hpp"

void *thread_bind(void *arg)
{
	Socket::Client *client = static_cast<Socket::Client *>(arg);
	client->f(client->fd);
	return (NULL);
}


Socket::Socket(const char *ip, int port, Socket::socket_mode mode) : _mode(mode), _port(port), _ip(ip)
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd <= 0)
		throw std::runtime_error("socket() failed");

	_addr.sin_addr.s_addr = inet_addr(ip);
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
}

Socket::~Socket()
{
	for (std::vector<Client *>::iterator it = _threads.begin(); it != _threads.end(); it++)
	{
		Client *client = *it;
		pthread_join(client->tid, NULL);
		std::cout << "deleting" << client->fd << std::endl;
		delete client;
	}
	close(_socket_fd);
}

void Socket::listening()
{
	if (_mode != SERVER)
		throw std::runtime_error("listening() failed, Socket is not in server mode");

	bind(_socket_fd, (const struct sockaddr *)&_addr, sizeof(_addr));
	if (listen(_socket_fd, 5) < 0)
		throw std::runtime_error("listen() failed");
	std::cout << "listen on " << _ip << ":" << _port << "" << std::endl;
}

void Socket::connecting()
{
	if (_mode != CLIENT)
		throw std::runtime_error("connecting() failed, Socket is not in client mode");

	if (connect(_socket_fd, (const struct sockaddr *)&_addr, sizeof(_addr)) < 0)
		throw std::runtime_error("connect() failed");
	std::cout << "connect on " << _ip << ":" << _port << "" << std::endl;
}


int Socket::create_client(void (*f)(int client_fd))
{
	if (_mode != SERVER)
		throw std::runtime_error("create_client() failed, Socket is not in server mode");

	sockaddr_in client_addr;
	socklen_t csize = sizeof(client_addr);
	int client_fd = accept(fd(), (struct sockaddr*)&client_addr, &csize);

	std::cout << "accept() success" << std::endl;
	std::cout << "new client: " << client_fd << std::endl;

	// pthread_t tid;
	Client *client = new Client();

	client->fd = client_fd;
	client->f = f;

	if (pthread_create(&client->tid, NULL, thread_bind, (void *)client) != 0)
		throw std::runtime_error("pthread_create() failed");
	_threads.push_back(client);

	return client_fd;
}

int Socket::fd() const
{
	return (_socket_fd);
}