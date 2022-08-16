#include "./client.hpp"

bool is_client_key_colliding(Client& client, const char*key, std::string& next)
{
	for (std::map<int, Client>::const_iterator it = Server::clients.begin(); it != Server::clients.end(); it++)
	{
		if (client.get_fd() != it->second.get_fd() && next == it->second.get_key(key))
			return true;
	}
	return false;
}

Client* get_client_by_key(const char* key, const char* value)
{
	for (std::map<int, Client>::iterator it = Server::clients.begin(); it != Server::clients.end(); it++)
	{
		if (it->second.get_key(key) == value)
			return &it->second;
	}
	return NULL;
}

Client* get_client_by_fd(int fd)
{
	std::map<int, Client>::iterator it = Server::clients.find(fd);
	if (it != Server::clients.end())
		return &it->second;
	return NULL;
}


bool is_not_target(Client &initiator, Client &target)
{
	if (initiator.get_fd() == target.get_fd())
		return false;
	return true;
}