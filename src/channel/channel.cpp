#include "channel.hpp"

Operator::Operator(): _prefix(std::vector<char>()) {}

void Operator::give(std::string prefix)
{
	size_t i = 0;
	while (i < prefix.size())
	{ 
		_prefix.push_back(prefix[i]);
		i++;
	}
}

void Operator::take(std::string prefix)
{
	size_t i = 0;
	while (i < prefix.size())
	{ 
		utils::remove_if(_prefix, prefix[i]);
		i++;
	}
	
}

Channel::Channel (std::string name, std::string password): _name(name), _password(password)
{}

Operator& Channel::insert(Client &client)
{
	std::pair<std::map<int, Operator>::iterator, bool> it = _clients.insert(std::make_pair(client.get_fd(), Operator()));
	if (!it.second)
		throw std::bad_alloc();
	client.join(_name);
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

std::string Channel::get_topic() const
{
	return _topic;
}

int	Channel::connected_clients() const
{
	return _clients.size();
}

bool Channel::include(int client_fd) const
{
	std::map<int, Operator>::const_iterator it = _clients.find(client_fd);
	if (it == _clients.end())
		return false;
	return true;
}

void Channel::kick(int client_fd)
{
	std::map<int, Operator>::iterator it = _clients.find(client_fd);
	if (it != _clients.end())
		_clients.erase(it);
}