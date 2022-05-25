/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 14:18:22 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/25 07:53:59 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../irc.hpp"

class Channel
{
public :
	Channel(void);
	~Channel(void){};
	void					add_client(Client* client);
	void					set_topic(std::string uuid);
	std::vector<char>		get_mode(){return (_mode);};
	size_t					get_max_client(){return (_max_client);};
	std::vector<Client*>	get_black_liste(){return (_black_list);};
	std::vector<Client*>	get_clients(){return (_clients);};
	std::string				get_topic(){return (_topic);};
	std::string				get_password(){return (_password);};
	void					print_clients();
private :
	std::vector<Client*>		_clients;
	std::vector<Client*>		_black_list;
	std::string					_topic;
	std::string					_password;
	std::vector<char>			_mode;	//il faux fair un tableaux, il peux y en avoir plusieur
	int							_max_client;
};

#endif