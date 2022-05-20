#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../irc.hpp"

class Command
{
public:
	typedef std::pair<std::string, std::vector<std::string> > command_t;

	Command();

	static command_t parse(const std::string& str);

private:


};

#endif