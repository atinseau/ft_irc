/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:35 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 17:00:32 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../irc.hpp"

class Command
{
	struct Payload
	{
		Payload(Client& client, const std::vector<Client>& clients, const Request::Body& body, std::map<std::string, Channel>& channels);

		Client &client;
		const std::vector<Client>& clients;
		const Request::Body &body;
		std::map<std::string, Channel>& channels;
		
	};

	typedef void (*func_t)(Payload);
	typedef bool (*func_t2)(Channel channel, std::vector<std::string> cmd);
	typedef std::map<std::string, func_t> map_t;
	

public:
	Command(Client &client, std::vector<Client> &clients);

	void ex_cmd(const Request::Body &body, std::map<std::string, Channel>& channels);

	static void nick(Payload p);
	static void pass(Payload p);
	static void user(Payload p);
	static void join(Payload p);

	// static bool mode_o(Channel channel, std::vector<std::string> cmd);
	// static bool mode_p(Channel channel, std::vector<std::string> cmd);
	// static bool mode_s(Channel channel, std::vector<std::string> cmd);
	// static bool mode_i(Channel channel, std::vector<std::string> cmd);
	// static bool mode_t(Channel channel, std::vector<std::string> cmd);
	// static bool mode_n(Channel channel, std::vector<std::string> cmd);
	// static bool mode_m(Channel channel, std::vector<std::string> cmd);
	// static bool mode_l(Channel channel, std::vector<std::string> cmd);
	// static bool mode_b(Channel channel, std::vector<std::string> cmd);
	// static bool mode_v(Channel channel, std::vector<std::string> cmd);
	static bool mode_k(Channel channel, std::vector<std::string> cmd);

	class ResponseException : public std::exception
	{
	public:
		ResponseException(Response res);
		~ResponseException() throw();

		virtual const char *what() const throw();
		Response &response();

	protected:
		Response _res;
	};

	class AuthException : public ResponseException
	{
	public:
		AuthException(Response res);
	};

private:
	static map_t init_cmd();
	static map_t _commands;
	static std::map<char, func_t2> init_mode();
	static std::map<char, func_t2> _mode;


	Client &_client;
	std::vector<Client> &_clients;
}; 

#endif