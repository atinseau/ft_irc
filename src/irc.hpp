/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:29 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 10:54:51 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

// in english les variables please ;)
// # define BLEU_2 "\e[1;34m"

#define SERVER_PORT 10000
#define TIME 3 * 60 * 1000

# define CLEAR "\e[0m"

# define RED "\e[1;31m"
# define YELLOW "\e[1;33m"
# define BLUE "\e[1;36m"
# define PURPLE "\e[1;35m"
# define GREEN "\e[1;32m"

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
#include <unistd.h>
#include <map>
#include <vector>
#include <map>

class Client;
class Socket;
class Command;

#include "utils/utils.hpp"
#include "command/command.hpp"
#include "client/client.hpp"
#include "socket/socket.hpp"


#endif