#include "../command.hpp"

void Command::pass(Payload p)
{
	if (p.client["PASSWORD"].size() > 0)
		throw ResponseException(ERR_ALREADYREGISTRED(p.client["NICKNAME"]));
	if (p.body.second.size() != 1)
		throw ResponseException(ERR_NEEDMOREPARAMS(p.client["NICKNAME"], p.body.first));

	if (p.body.second[0] != Server::password)
		throw ResponseException(ERR_INVALIDPASS);
	p.client["PASSWORD"] = p.body.second[0];
}