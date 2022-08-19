/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:18:22 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 10:06:49 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../irc.hpp"

class Mode
{
public:

	Mode();

	void give(std::string modes);
	void take(std::string modes);

	bool has(char mode) const;
	const std::vector<char>& get_modes() const;

private:
	std::vector<char> _modes;
};

class Operator: public Mode
{
public:
	Operator();
};

struct DispatchFilter
{
	bool (*filter)(Client &);
	Client &source;
};

class Channel: public Mode
{
private:
	std::map<int, Operator> _clients;
	std::vector<char> _modes;
	std::string _name;
	std::string _topic;
	std::string _password;
	int _limit;

public:
	class Dispatcher
	{
	public:
		Dispatcher(Channel *_channel, Client *initiator = NULL);

		void load(std::string message);
		void load(std::vector<std::string> messages);

		void send(bool (*filter)(Client& initiator, Client& target) = NULL);

	private:
		Channel *_channel;
		Client *_initiator;
		std::vector<std::string> _messages;
	};

	Channel(std::string name, std::string password = "");

	Operator &insert(Client &client);

	const std::string& get_password() const;
	const std::string& get_name() const;
	const std::string& get_topic() const;
	int get_limit() const;

	int connected_clients() const;

	bool include(int client_fd) const;
	void kick(int client_fd);
	std::string get_client_list() const;

	Dispatcher create_dispatcher(Client *initiator = NULL);

	void set_topic(const std::string& topic);
	void set_password(const std::string& password);
	void set_limit(int limit);
};

std::string fix_channel_name(const std::string &name);
bool operator_is(const Operator &op, char p);

#endif