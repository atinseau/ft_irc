#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../irc.hpp"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <netinet/in.h>

#include <unistd.h>
#include <errno.h>

#define SERVER_PORT 10000

class Server
{
public:
	Server(void);
	~Server(void);

	void run(void);

private:
	int _sock;
	struct sockaddr_in6   _addr;
};

#endif