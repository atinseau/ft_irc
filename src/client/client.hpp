/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:53:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/31 16:21:38 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../irc.hpp"

# define NICKNAME_LENGTH 9

class Client
{
public:
	Client(pollfd pfd);

	void								disconnect(std::map<std::string, Channel> *Channels);
	void								disconnect_channel(Channel *channel, std::map<std::string, Channel> *all_channels);
	Request								read();
	void								write(Response res);

	int 								get_fd() const;
	pollfd 								get_pfd();
	std::map<std::string, Channel*>&	get_channels();
	std::map<char,bool>					get_mode(Channel* channel);
	bool 								is_auth();
	std::string&						operator[](const char* key);
	std::string							get_key(const char* key) const;
	void								print_channel();
	void 								add_channels(std::pair<std::string , Channel* > channel);
	void								add_mode(Channel *channel);
	void 								print_mode_by_channel(Channel *channel);

	static std::string server_password;
private:
	std::map<std::string, Channel*>				_channels;
	std::map<Channel*, std::map<char,bool> >	_modes;
	pollfd										_pfd;
	std::map<const char *, std::string>			_data;
};

#endif