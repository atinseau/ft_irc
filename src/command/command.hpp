/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:35 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 08:07:09 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../irc.hpp"

class Command
{
	struct Payload
	{
		Payload(Client& client, std::map<int,Client> &clients, const Request::Body& body, std::map<std::string, Channel>& channels);

		Client &client;
		std::map<int,Client> &clients;
		const Request::Body &body;
		std::map<std::string, Channel>& channels;
		
	};

	typedef void (*func_t)(Payload);
	typedef std::map<std::string, func_t> map_t;
public:
	Command(Client &client, std::map<int,Client> &clients);

	void ex_cmd(const Request::Body &body, std::map<std::string, Channel>& channels);


	static Client 			*get_client_by_nickname(std::string nickname, std::map<int, Client> &clients);
	static std::string		return_str_client_channel(std::map<std::string, Channel*> channels, Client &client);
	static void nick(Payload p);
	static void pass(Payload p);
	static void user(Payload p);
	static void join(Payload p);
	static void part(Payload p);
	static void mode(Payload p);
	static void topic(Payload p);
	static void quit(Payload p);
	static void names(Payload p);
	static void list(Payload p);
	static void invite(Payload p);
	static void kick(Payload p);
	static void privmsg(Payload p);
	static void oper(Payload p);

	static void help(Payload p);

private:
	static map_t init_cmd();
	static map_t _commands;
	

	Client &_client;
	std::map<int,Client> &_clients;
}; 

#endif