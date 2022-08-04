/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:52 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/31 09:24:34 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "../irc.hpp"

Request::Request() : request_t(0, ""), _type(NONE), _ready(false) {}
Request::Request(int n, char *s) : request_t(n, s), _ready(false) {}

request_t::first_type Request::size() const
{
	return (this->first);
}



std::vector<Request::Body> Request::body() const
{
	return (get_bodies(this->second));
}

Request::Type Request::type() const
{
	return (this->_type);
}

void Request::set_type(Type t)
{
	this->_type = t;
}

bool Request::is_ready() const
{
	return (this->_ready);
}

void Request::set_ready(bool b)
{
	this->_ready = b;
}

std::ostream &operator<<(std::ostream &os, const Request::Body &body)
{
	LOG(YELLOW, os, "|" << body.first << "|");
	for (std::vector<std::string>::const_iterator it = body.second.begin(); it != body.second.end(); it++)
		LOG(YELLOW, os, "|" << *it << "|");
	return (os);
}