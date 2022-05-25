/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:32 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/25 19:02:26 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

Channel::ResponseException::ResponseException(Response res) : _res(res)
{}
Channel::ResponseException::~ResponseException() throw()
{}
const char *Channel::ResponseException::what() const throw()
{
	return "ResponseException";
}
Response& Channel::ResponseException::response()
{
	return (_res);
}
Command::AuthException::AuthException(Response res) : ResponseException(res) {}