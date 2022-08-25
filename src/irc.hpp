/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:29 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 08:07:32 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

// in english les variables please ;)
// # define BLEU_2 "\e[1;34m"

#define SERVER_PORT 10000
#define TIME 100 * 60 * 1000

#ifndef DEV
 # define DEV 0
#endif

# define CLEAR "\e[0m"
# define RED "\e[1;31m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;36m"
# define PURPLE "\e[1;35m"
# define GREEN "\e[1;32m"

// # define CLEAR ""
// # define RED ""
// # define YELLOW ""
// # define BLUE ""
// # define PURPLE ""
// # define GREEN ""

# define LOG(color, output, msg) output << color << msg << CLEAR << std::endl

# define ERROR(msg) LOG(RED, std::cerr, msg)
# define WARNING(msg) LOG(YELLOW, std::cerr, msg)
# define INFO(msg) LOG(BLUE, std::cout, msg)
# define DEBUG(msg) LOG(PURPLE, std::cout, msg)
# define SUCCESS(msg) LOG(GREEN, std::cout, msg) 

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <map>
#include <sys/time.h>

#include "utils/utils.hpp"
#include "request/request.hpp"
#include "response/response.hpp"


/**
 * @brief 
 * Forwards declaration
 */
class Response;
class ResponseException;
class AuthException;
class Socket;
class Channel;
class Client;
class Response;
class Command;

#include "socket/socket.hpp"
#include "client/client.hpp"
#include "channel/channel.hpp"
#include "response/response.hpp"
#include "command/command.hpp"


template <typename T>
std::ostream& operator<<(std::ostream& output, const std::vector<T>& vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		output << vec[i];
		if (i != vec.size() - 1)
			output << ",";
	}
	return output;
}


#endif