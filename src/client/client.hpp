/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:53:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/19 10:37:43 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../irc.hpp"

class Client
{
public:
	typedef std::pair<int, std::string> request_t;

	class Request : public request_t
	{
	public:
		// enum Type
		// {
		// 	QUIT,
		// 	NONE
		// };

		Request();
		explicit Request(int n, char *s);

		request_t::first_type size() const;
		request_t::second_type body() const;

		// Type type() const;

	// private:
	// 	Type _type;
	};

	Client(pollfd *pfd);

	void disconnect();
	int fd() const;
	Request read();

private:
	pollfd *_pfd;

	// std::string _username;
	// std::string _nickname;
	// std::string _channel;
	// std::string _password;
};

#endif