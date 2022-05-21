#include "request.hpp"
#include "../irc.hpp"

Request::Request(): request_t(0, ""), _type(NONE) {}
Request::Request(int n, char *s): request_t(n, s) {}

request_t::first_type Request::size() const
{
	return (this->first);
}

Request::Body Request::body() const
{
	Body b;

	std::vector<std::string> args = split(this->second.c_str(), ' ');
	if (args.size() == 0)
		throw std::runtime_error("La commande est vide, cas de figure non géré");
	
	b.first = args[0];
	args.erase(args.begin());

	std::string& end = args.back();
	end.pop_back();
	end.pop_back();

	b.second = args;
	return (b);
}

Request::Type Request::type() const
{
	return (this->_type);
}

void Request::set_type(Type t)
{
	this->_type = t;
}

std::ostream& operator<<(std::ostream& os, const Request::Body& body)
{
	LOG(YELLOW, os, "|" << body.first << "|");
	for (std::vector<std::string>::const_iterator it = body.second.begin(); it != body.second.end(); it++)
		LOG(YELLOW, os, "|" << *it << "|");
	return (os);
}