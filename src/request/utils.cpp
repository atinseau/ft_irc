#include "./request.hpp"
#include "../irc.hpp"

Request::Body get_body(const std::string &buffer)
{
	Request::Body b;

	std::vector<std::string> args = split(buffer.c_str(), ' ');
	if (args.size() == 0)
		throw std::runtime_error("La commande est vide, cas de figure non géré");

	b.first = args[0];
	args.erase(args.begin());
	b.second = args;

	for (std::vector<std::string>::iterator it = b.second.begin(); it != b.second.end(); it++)
	{
		while(isspace(it->at(it->size() - 1)))
			it->erase(it->size() - 1);
	}

	return (b);
}

std::vector<Request::Body> get_bodies(const std::string &buffer)
{
	std::vector<Request::Body> bodies;
	std::vector<std::string> lines = split(buffer.c_str(), '\n');

	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		if (!it->size())
			continue;
		bodies.push_back(get_body(*it));
	}

	return (bodies);
}