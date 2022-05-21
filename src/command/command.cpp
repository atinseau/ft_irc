#include "command.hpp"

using namespace std;

Command::Command(Client& client): _client(client) {}

void Command::operator[](const std::string& cmd)
{
	(void) _client;

	map_t::iterator it = _commands.find(cmd);

	if (it != _commands.end())
	{
		INFO("Commande " << it->first << " trouvée");
		std::cout << std::boolalpha;
		std::cout << _client.is_auth("") << std::endl;
	}
}


void Command::nick()
{
	std::cout << "nick" << std::endl;
}

Command::map_t Command::init()
{
	map_t map;

	map["NICK"] = &Command::nick;

	return (map);
}

Command::map_t Command::_commands = Command::init();

