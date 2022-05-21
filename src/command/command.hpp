#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../irc.hpp"

class Command
{
	typedef void (* func_t)(void); 
	typedef std::map<std::string, func_t> map_t;

public:

	Command(Client& client);

	void operator[](const std::string& cmd);

	static void nick();

private:
	static map_t init();
	static map_t _commands;

	Client& _client;
};

#endif