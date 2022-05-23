/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:32 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 15:41:30 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

Command::Payload::Payload(Client &client, const std::vector<Client>& clients, const Request::Body& body, std::map<std::string, Channel>& channels) : client(client), clients(clients), body(body), channels(channels) {}

Command::ResponseException::ResponseException(Response res) : _res(res){}

Command::ResponseException::~ResponseException() throw(){}

const char *Command::ResponseException::what() const throw()
{
	return "ResponseException";
}

Response& Command::ResponseException::response()
{
	return (_res);
}


Command::AuthException::AuthException(Response res) : ResponseException(res) {}