#include "../command.hpp"


void Command::quit(Payload p)
{
	throw std::runtime_error("Disconnecting from server");
}