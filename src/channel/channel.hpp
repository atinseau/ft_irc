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

class Operator
{
public:
	Operator();
	void give(std::string prefix);
	void take(std::string prefix);
private:
	std::vector<char> _prefix;
};

class Channel
{

public:
	Channel (std::string name, std::string password = "");
	
	Operator& insert(int client_fd);

	std::string get_password() const;
	std::string get_name() const;

	std::map<int, Operator> _clients;
private:
	std::string _name;
	std::string _topic;
	std::string _password;
};

#endif