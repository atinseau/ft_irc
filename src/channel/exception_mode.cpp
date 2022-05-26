/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception_mode.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:32 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/26 08:36:05 by mbonnet          ###   ########.fr       */
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
Channel::AuthException::AuthException(Response res) : ResponseException(res) {}