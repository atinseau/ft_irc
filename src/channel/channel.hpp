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

class Channel
{
	typedef void (*func_t1)(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static std::map<char, func_t1> init_mode_join();
	typedef void (*func_t2)(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels);
	static std::map<char, func_t2> init_mode_mode();
public :
	Channel(void);
	~Channel(void){};
	void					add_client(Client *client);
	void					set_topic(std::string uuid);
	size_t					sup_client(int cfd);
	std::map<char,bool>		get_mode(){return (_mode);};
	size_t					get_max_client(){return (_max_client);};
	std::map<int, Client*>	get_clients(){return (_clients);};
	std::string				get_topic(){return (_topic);};
	std::string				get_password(){return (_password);};
	void					print_clients(std::string name_channel);
	void					set_mode(char choose, char mode);
	void					set_password(std::string pass){_password = pass;};
	void					set_max_client(int nb){_max_client = nb;};
	void					send_msg_all_client(Client *client, std::string msg);

	static void join_o(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_pi(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_t(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_l(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_k(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static std::map<char, func_t1> _mode_join;
	
	static void mode_o(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels);
	static void mode_pit(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels);
	static void mode_l(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels);
	static void mode_k(Channel *channel, char choose, char mode, std::string para, Client *client, std::map<int, Client> *clients, std::map<std::string, Channel> *channels);
	static std::map<char, func_t2> _mode_mode;


private :
	std::map<int, Client*>		_clients;
	std::string					_topic;
	std::string					_password;
	std::map<char,bool>			_mode;
	int							_max_client;
};

#endif