#ifndef USERS_HPP
#define USERS_HPP

#include "../irc.hpp"

struct User: pollfd
{
	User();
	std::string nick;
	std::string user;
};


// void registration(const Socket::Client *client);

#endif