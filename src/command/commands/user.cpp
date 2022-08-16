#include "../command.hpp"

void Command::user(Payload p)
{
	if (p.body.second.size() < 4)
		throw ResponseException(ERR_NEEDMOREPARAMS("*", "USER"));
	if (p.client.get_key("USERNAME") != "")
		throw ResponseException(ERR_ALREADYREGISTRED(p.client.get_key("USERNAME")));
	p.client["USERNAME"] = p.body.second[0];
	p.client["REALNAME"] = p.body.second[3];
}
