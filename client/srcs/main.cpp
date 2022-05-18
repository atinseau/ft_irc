/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 10:11:23 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 18:18:02 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../src/irc.hpp"


int	main()
{
	int on = 1;
	int	socketClient = socket(AF_INET6, SOCK_STREAM, 0);
	pollfd fd;
	if ((ioctl(socketClient, FIONBIO, (char *)&on)) < 0)
		throw std::runtime_error(ROUGE"ioctl() failed"BLANC);
	struct sockaddr_in6 addrClient;
	memset(&addrClient, 0, sizeof(addrClient));
  	memcpy(&addrClient.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addrClient.sin6_family      = AF_INET6;
	addrClient.sin6_port = htons(10000);
	connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
	std::cout << "connected" << std::endl;
	std::string test;
	char str[5];
	fd.fd = socketClient;
	fd.events = POLLIN;
	while (1)
	{
		if ((poll(&(fd), sizeof(fd), TIME)) <= 0)
			throw std::runtime_error(ROUGE"poll() failed/timeout"BLANC);
		std::getline(std::cin, test);
		test[test.size()] = '\0';
		send(socketClient, test.c_str(), test.size(), 0);
		if (test == "/end")
		{
			recv(socketClient, str, sizeof(str), 0);
			str[4] = '\0';
			test = str;
			if (test == "/end")
				break;
		}
	}
	close(socketClient);
	return (0);
}