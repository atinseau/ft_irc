#include "socket.hpp"

// SOCKET CLASS

Socket::Socket(const char *ip, int port, Socket::socket_mode mode, const char **envp) : _mode(mode), _port(port), _ip(ip), _envp(envp), _password(NULL)
{
	(void)_envp;

	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd <= 0)
		throw std::runtime_error("socket() failed");

	_addr.sin_addr.s_addr = inet_addr(ip);
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);

	_pfds.push_back(User());
	_pfds.back().fd = _socket_fd;
	_pfds.back().events = POLLIN;
}

Socket::~Socket() {}

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

void handle(int i)
{
	char buffer[1024];
	int n = recv(i, buffer, 1024, 0);
	buffer[n] = '\0';
	std::cout << buffer << std::endl;
}

void Socket::run()
{
	loop
	{
		if (poll(static_cast<pollfd*>(&_pfds[0]), _pfds.size(), 0) < 0)
			throw std::runtime_error("poll() failed");
		if (_pfds[0].revents == POLLIN) {
			create_client();
		}
	
		for (std::vector<User>::iterator it = _pfds.begin(); it != _pfds.end(); ++it)
		{
			if (it->revents == POLLIN && it->fd != _socket_fd)
			{
				handle(it->fd);
			}
		}
	}
}

int Socket::create_client()
{
	if (_mode != SERVER)
		throw std::runtime_error("create_client() failed, Socket is not in server mode");

	sockaddr_in client_addr;
	socklen_t csize = sizeof(client_addr);
	int client_fd = accept(fd(), (struct sockaddr *)&client_addr, &csize);

	if (client_fd < 0)
		return (-1);

	std::cout << "accept() success" << std::endl;
	std::cout << "new client: " << client_fd << std::endl;

	_pfds.push_back(User());
	_pfds.back().fd = client_fd;
	_pfds.back().events = POLLIN;
	return (client_fd);
}

// SETTER AND GETTER

int Socket::fd() const
{
	return (_socket_fd);
}

void Socket::setPassword(const char *password)
{
	_password = strdup(password);
}

// STATIC

// static int recv(int fd, char *buffer, int size)
// {
// 	memset(buffer, 0, size);
// 	int n = recv(fd, buffer, size, 0);
// 	if (n <= 0)
// 		throw std::runtime_error("recv() failed");
// 	buffer[n] = '\0';
// 	return (n);
// }