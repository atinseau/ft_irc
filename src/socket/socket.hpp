#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <poll.h>

#include "../irc.hpp"
#include "../define.hpp"

struct User;

void *thread_bind(void *arg);

class Socket
{
	typedef struct sockaddr_in sockaddr_in;
	typedef bool socket_mode;

public:
	Socket(const char *ip, int port, socket_mode mode, const char **envp = NULL);
	~Socket();

	// Static
	// static int recv(int fd, char *buffer, int size);

	// SERVER FUNCTION
	void listening();
	void setPassword(const char *password);
	void run();
	int create_client();

	// CLIENT FUNCTION
	void connecting();

	// GLOBAL FUNCTION
	int fd() const;

private:
	sockaddr_in _addr;
	socket_mode _mode;
	std::vector<User> _pfds;

	int _socket_fd;
	int _port;
	const char *_ip;
	const char **_envp;
	char *_password;
};

#endif