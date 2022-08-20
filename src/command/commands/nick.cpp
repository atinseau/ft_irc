#include "../command.hpp"

void dispatch_new_nick(Client& client, const std::string& old_nickname, const std::string& new_nickname)
{
	std::vector<Channel*> client_channels = client.get_channels();
	for (std::vector<Channel*>::iterator it = client_channels.begin(); it != client_channels.end(); it++)
	{
		Channel::Dispatcher dispatcher = (*it)->create_dispatcher(&client);
		dispatcher.load(RPL_NICKCHANGE(old_nickname, new_nickname));
		dispatcher.send(is_not_target);
	}
	client.write(RPL_NICKCHANGE(old_nickname, new_nickname));
}

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
	const std::string& old_nickname = p.client.get_key("NICKNAME");
	if (!old_nickname.empty() && old_nickname != nickname)
		dispatch_new_nick(p.client, old_nickname, nickname);
	p.client["NICKNAME"] = nickname;
}


