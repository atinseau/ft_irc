#include "../command.hpp"

void Command::ping(Payload p)
{
	(void)p;
	// p.client.write(RPL_PING(p.client["USERNAME"]));
}

void Command::pong(Payload p)
{
	(void)p;
	// p.client.write(RPL_PONG(Server::name));
	// p.client.write()
}

void Command::quit(Payload p)
{
	(void)p;
	throw std::runtime_error("Disconnecting from server");
}

void Command::whois(Payload p)
{
	(void)p;
	// p.client.write(RPL_ENDOFWHOIS(p.client["USERNAME"]));
}

void Command::userhost(Payload p)
{
	// p.client.write(RPL_USERHOST(p.client["USERNAME"], p.client["NICKNAME"]));
	(void)p;
}

void Command::lusers(Payload p)
{
	// p.client.write(RPL_LUSERCLIENT(p.client["USERNAME"]));
	(void)p;
}

void Command::cap(Payload p)
{
	(void)p;
}