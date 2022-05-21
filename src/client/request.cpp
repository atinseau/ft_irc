#include "client.hpp"

Client::Request::Request(): request_t(0, ""), _type(NONE) {}
Client::Request::Request(int n, char *s): request_t(n, s) {}

Client::request_t::first_type Client::Request::size() const
{
	return (this->first);
}

Client::Request::Body Client::Request::body() const
{
	Body b;

	std::vector<std::string> args = split(this->second.c_str(), ' ');
	if (args.size() == 0)
		throw std::runtime_error("La commande est vide, cas de figure non géré");
	
	b.first = args[0];
	args.erase(args.begin());
	(args.end() - 1)->pop_back();
	b.second = args;
	return (b);
}

Client::Request::Type Client::Request::type() const
{
	return (this->_type);
}

void Client::Request::set_type(Type t)
{
	this->_type = t;
}
