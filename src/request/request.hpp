/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:54 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 12:10:55 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../utils/utils.hpp"

typedef std::pair<int, std::string> request_t;

class Request : public request_t
{

public:
	typedef std::pair<std::string, std::vector<std::string> > Body;
	enum Type
	{
		QUIT,
		NONE,
		ERROR,
		SUCCESS
	};

	Request();
	explicit Request(int n, char *s);

	request_t::first_type size() const;

	Type type() const;
	std::vector<Body> body() const;
	bool is_ready() const;

	void set_type(Type t);
	void set_ready(bool b);

private:
	Type _type;
	bool _ready;
};


std::ostream& operator<<(std::ostream& os, const Request::Body& body);

Request::Body get_body(const std::string &buffer);
std::vector<Request::Body> get_bodies(const std::string &buffer);

#endif