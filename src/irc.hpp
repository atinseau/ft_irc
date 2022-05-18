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

# define BLEU "\e[1;36m"
# define BLEU_2 "\e[1;34m"
# define ROUGE "\e[1;31m"
# define VERT "\e[1;32m"
# define JAUNE "\e[1;33m"
# define VIOLET "\e[1;35m"
# define BLANC "\e[0m"
#define SERVER_PORT 10000
#define TIME 3 * 60 * 1000

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

class Client;
class Socket;

#include "utils/utils.hpp"
#include "client/client.hpp"
#include "socket/socket.hpp"


#endif