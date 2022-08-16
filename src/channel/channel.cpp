#include "channel.hpp"


Operator::Operator() : Mode() {}


Channel::Channel(std::string name, std::string password) : Mode(), _name(name), _password(password)
{}

Operator &Channel::insert(Client &client)
{
	std::pair<std::map<int, Operator>::iterator, bool> it = _clients.insert(std::make_pair(client.get_fd(), Operator()));
	if (!it.second)
		throw std::bad_alloc();
	client.join(_name);
	return it.first->second;
}

const std::string& Channel::get_password() const
{
	return _password;
}

const std::string& Channel::get_name() const
{
	return _name;
}

const std::string& Channel::get_topic() const
{
	return _topic;
}

int Channel::get_limit() const
{
	return _limit;
}

int Channel::connected_clients() const
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



std::string Channel::get_client_list() const
{
	std::string list;
	size_t i = 0;
	for (std::map<int, Operator>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *client = get_client_by_fd(it->first);
		if (!client)
			continue;
		if (operator_is(it->second, 'o'))
			list += "@";
		list += client->get_key("NICKNAME") + (i < _clients.size() - 1 ? " " : "");
		i += 1;
	}
	return list;
}

Channel::Dispatcher Channel::create_dispatcher(Client* initiator)
{
	Dispatcher dispatcher(this, initiator);
	return dispatcher;
}

Channel::Dispatcher::Dispatcher(Channel* channel, Client *initiator) : _channel(channel), _initiator(initiator)
{}

void Channel::Dispatcher::load(std::string message)
{
	_messages.push_back(message);
}

void Channel::Dispatcher::load(std::vector<std::string> messages)
{
	for (std::vector<std::string>::const_iterator it = messages.begin(); it != messages.end(); it++)
		_messages.push_back(*it);
}

void Channel::Dispatcher::send(bool (*filter)(Client& initiator, Client& target))
{
	for (std::map<int, Operator>::const_iterator it = _channel->_clients.begin(); it != _channel->_clients.end(); it++)
	{
		Client* client = get_client_by_fd(it->first);
		if (!client)
			continue;
		if (filter && _initiator && !filter(*_initiator, *client))
			continue;
		for (std::vector<std::string>::const_iterator it2 = _messages.begin(); it2 != _messages.end(); it2++)
			client->write(*it2);
	}
	_messages.clear();
}

void Channel::set_topic(const std::string& topic)
{
	_topic = topic;
}

void Channel::set_password(const std::string& password)
{
	_password = password;
}

void Channel::set_limit(int limit)
{
	_limit = limit;
}