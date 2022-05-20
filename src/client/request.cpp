#include "client.hpp"

Client::Request::Request(): request_t(0, "") {}
Client::Request::Request(int n, char *s): request_t(n, s) {}

Client::request_t::first_type Client::Request::size() const
{
	return (this->first);
}

Client::request_t::second_type Client::Request::body() const
{
	return (this->second);
}
