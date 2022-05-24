/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:53:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 10:48:06 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../irc.hpp"

# define NICKNAME_LENGTH 9

class Client
{
public:
	Client(pollfd *pfd);

	void disconnect();
	Request read();
	void write(Response res);

	int get_fd() const;
	pollfd* get_pfd();

	bool is_auth();

	std::string& operator[](const char* key);
	std::string get_key(const char* key) const;
	void add_channel(Channel* channel);

	static std::string server_password;

private:
	std::vector<Channel *>				_channels;
	pollfd								*_pfd;
	std::map<const char *, std::string>	_data;
};

#endif