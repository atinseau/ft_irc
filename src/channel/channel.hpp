/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:18:22 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/26 15:47:43 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../irc.hpp"

class Channel
{
	typedef void (*func_t1)(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static std::map<char, func_t1> init_mode_join();
public :
	Channel(void);
	~Channel(void){};
	void					add_client(Client client);
	void					set_topic(std::string uuid);
	size_t					sup_client(int cfd);
	std::map<char,bool>		get_mode(){return (_mode);};
	size_t					get_max_client(){return (_max_client);};
	std::vector<Client*>	get_black_liste(){return (_black_list);};
	std::map<int, Client>	get_clients(){return (_clients);};
	std::string				get_topic(){return (_topic);};
	std::string				get_password(){return (_password);};
	void					print_clients();

	static void join_o(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_psi(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_t(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_n(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_m(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_l(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_b(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_v(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static void join_k(Channel &channel, std::vector<std::string>& cmd, std::vector<std::string>& para, Client &client);
	static std::map<char, func_t1> _mode_join;

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

private :
	std::map<int, Client>		_clients;
	std::vector<Client*>		_black_list;
	std::string					_topic;
	std::string					_password;
	std::map<char,bool>			_mode;
	int							_max_client;
};

#endif