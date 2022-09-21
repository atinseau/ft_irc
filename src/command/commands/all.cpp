#include "../command.hpp"

void Command::ping(Payload& p)
{
	(void)p;
}

void Command::pong(Payload& p)
{
	(void)p;
}

void Command::quit(Payload& p)
{
	(void)p;
	throw std::runtime_error("Disconnecting from server");
}

void Command::whois(Payload& p)
{
	(void)p;
}

void Command::userhost(Payload& p)
{
	(void)p;
}

void Command::lusers(Payload& p)
{
	(void)p;
}

void Command::cap(Payload& p)
{
	(void)p;
}

void Command::who(Payload& p)
{
	(void)p;
}