/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:29 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/27 08:48:24 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response(std::string os): _str(os) {}

std::string Response::str() const
{
	return (_str);
}

ResponseException::ResponseException(std::string response) : _res(Response(response)){}

ResponseException::~ResponseException() throw(){}

const char *ResponseException::what() const throw()
{
	return "ResponseException";
}

Response& ResponseException::response()
{
	return (_res);
}

AuthException::AuthException(std::string response): ResponseException(response) {}