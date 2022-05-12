#include "socket.hpp"

Server::Server(void)
{
	this->_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->_sock < 0)
		throw std::runtime_error("socket() failed");

	int opt = 0;

	if (setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		close(this->_sock);
		throw std::runtime_error("setsockopt() failed");
	}

	if (ioctl(this->_sock, FIONBIO, (char *)&opt) < 0)
	{
		close(this->_sock);
		throw std::runtime_error("ioctl() failed");
	}

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin6_family = AF_INET6;
	memcpy(&_addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	_addr.sin6_port = htons(SERVER_PORT);
	if (bind(_sock, (struct sockaddr *)&_addr, sizeof(_addr)) < 0)
	{
		close(this->_sock);
		throw std::runtime_error("bind() failed");
	}
}

Server::~Server()
{
	close(this->_sock);
}