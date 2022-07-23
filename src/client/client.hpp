/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:53:33 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 15:25:54 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../irc.hpp"

# define NICKNAME_LENGTH 9

enum ClientState
{
	IS_AUTH,
	IS_NOT_AUTH,
	AFTER_AUTH
};

class Client
{
public:
	Client(pollfd pfd);
	~Client(void);

	void								disconnect();
	Request								read();
	void								write(Response res);
	int 								get_fd() const;
	pollfd 								get_pfd();
	bool 								is_auth();
	std::string& operator[](const char* key);
	std::string							get_key(const char* key) const;

	void								set_state(ClientState state);
	ClientState							get_state() const;

	std::vector<std::string> 			get_info(bool print = false) const;

private:
	pollfd								_pfd;
	std::map<const char*, std::string>	_data;
	ClientState							_state;
};

bool is_client_key_colliding(Client& client, const char* key, std::string& next);
Client* get_client_by_key(const char* key, const char* value);

#endif