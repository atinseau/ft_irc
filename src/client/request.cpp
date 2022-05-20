#include "client.hpp"

Client::Request::Request(): request_t(0, ""), _type(NONE) {}
Client::Request::Request(int n, char *s): request_t(n, s) {}

Client::request_t::first_type Client::Request::size() const
{
	return (this->first);
}

const Client::request_t::second_type& Client::Request::body() const
{
	return (this->second);
}

Client::Request::Type Client::Request::type() const
{
	return (this->_type);
}

void Client::Request::set_type(Type t)
{
	this->_type = t;
}
