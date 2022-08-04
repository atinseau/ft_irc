#include "../command.hpp"


void Command::whois(Payload p)
{
	DEBUG(p.body.second);
	p.client.write(RPL_ENDOFWHOIS(p.client["USERNAME"]));
}