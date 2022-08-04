/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/06 08:30:52 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/irc.hpp"

void app(std::string port, std::string password)
{
	Server srv(port, password);
	srv.run();
}

int main(int ac, const char **av, const char **envp)
{
	(void)envp;

	std::string port;
	std::string password;
	try
	{
		if (ac != 3)
		{
			std::cout << "Argument incorecte ! <port> <password>" << std::endl;
			return (1);
		}
		else 
		{
			port = av[1];
			password = av[2];
		}
		app(port, password);
	}
	catch (const std::exception &e)
	{
		ERROR(e.what());
		return (1);
	}
	return (0);
}
