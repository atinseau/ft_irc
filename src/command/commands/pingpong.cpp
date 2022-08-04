#include "../command.hpp"

void Command::ping(Payload p)
{
	p.client.write(RPL_PING(p.client["USERNAME"]));
}

void Command::pong(Payload p)
{
	(void)p;
	p.client.write(RPL_PONG(Server::name));
	// p.client.write()
}