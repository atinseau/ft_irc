#include "channel.hpp"

Operator::Operator(): _prefix(std::vector<char>()) {}

void Operator::give(std::string prefix)
{
	int i = 0;
	while (i < prefix.size())
	{ 
		_prefix.push_back(prefix[i]);
		i++;
	}
}

void Operator::take(std::string prefix)
{
	int i = 0;
	while (i < prefix.size())
	{ 
		remove_if(_prefix, prefix[i]);
		i++;
	}
	
}

Channel::Channel (std::string name, std::string password): _name(name), _password(password)
{}

Operator& Channel::insert(int client_fd)
{
	std::pair<std::map<int, Operator>::iterator, bool> it = _clients.insert(std::make_pair(client_fd, Operator()));
	if (!it.second)
		throw std::bad_alloc();
	return it.first->second;
}

std::string Channel::get_password() const
{
	return _password;
}

std::string Channel::get_name() const
{
	return _name;
}
