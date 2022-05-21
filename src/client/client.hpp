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

typedef std::pair<int, std::string> request_t;

public:
	class Request : public request_t
	{
	public:

		typedef std::pair<std::string, std::vector<std::string> > Body;
		
		enum Type
		{
			QUIT,
			NONE,
			ERROR,
			SUCCESS
		};

		Request();
		explicit Request(int n, char *s);

		request_t::first_type size() const;

		Type type() const;
		Body body() const;
		
		void set_type(Type t);

	private:
		Type _type;
	};

	Client(pollfd *pfd);

	void disconnect();
	int get_fd() const;
	Request read();

	bool	is_auth(std::string pass);

	static std::string			server_password;

private:
	pollfd								*_pfd;
	std::map<const char *, std::string> _data;
	bool								_auth;

	// std::string						_username;
	// std::string				_nickname;
	// std::string _channel;
	// std::string _password;
};

#endif