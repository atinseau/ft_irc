#include "../command.hpp"

void Command::user(Payload& p)
{
	if (p.body.second.size() < 4)
		throw ResponseException(ERR_NEEDMOREPARAMS("*", "USER"));
	if (p.client.get_key("USERNAME") != "")
		throw ResponseException(ERR_ALREADYREGISTRED(p.client.get_key("USERNAME")));
	p.client["USERNAME"] = p.body.second[0];

	std::string realname;

	if (p.body.second[3][0] == ':') realname = p.body.second[3].substr(1);
	else realname = p.body.second[3];

	p.client["REALNAME"] = realname;
}
