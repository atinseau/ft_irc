#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <vector>

#define PORT 10000
#define IP "192.168.1.65"
#define SERVER true
#define CLIENT false

void *thread_bind(void *arg);

class Socket
{
	typedef struct sockaddr_in sockaddr_in;
	typedef bool socket_mode;

public:

	struct Client
	{
		pthread_t tid;
		int fd;
		void (*f)(int client_fd);
	};

	Socket(const char *ip, int port, socket_mode mode);
	~Socket();

	// SERVER FUNCTION
	void listening();
	int create_client(void (*f)(int client_fd));

	// CLIENT FUNCTION
	void connecting();

	// GLOBAL FUNCTION
	int fd() const;

private:
	sockaddr_in _addr;
	socket_mode _mode;
	int _socket_fd;
	int _port;
	const char *_ip;

	std::vector<Client *> _threads;

};