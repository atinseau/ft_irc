#include "../command.hpp"

void Command::nick(Payload p)
{
	if (p.body.second.size() == 0)
	{
		if (!p.client.is_auth())
			throw AuthException(ERR_NONICKNAMEGIVEN);
		throw ResponseException(ERR_NONICKNAMEGIVEN);
	}
	std::string nickname = p.body.second[0];
	// if (nickname[0] == ':')
	// 	nickname.erase(0);

	if (nickname.size() > NICKNAME_LENGTH)
	{
		if (!p.client.is_auth())
			throw AuthException(ERR_ERRONEUSNICKNAME(nickname));
		throw ResponseException(ERR_ERRONEUSNICKNAME(nickname));
	}

	if (is_client_key_colliding(p.client, "NICKNAME", nickname))
	{
		if (!p.client.is_auth())
			throw AuthException(ERR_NICKNAMEINUSE(nickname));
		throw ResponseException(ERR_NICKNAMEINUSE(nickname));
	}

	p.client["NICKNAME"] = nickname;
}


