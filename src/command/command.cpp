#include "command.hpp"

using namespace std;

Command::Payload::Payload(Client &client, const Request::Body &body) : client(client), body(body)
{
}

Command::Command(Client &client) : _client(client)
{
}

void Command::exec(const std::vector<Request::Body> &bodies)
{
	for (std::vector<Request::Body>::const_iterator it = bodies.begin(); it != bodies.end(); it++)
	{
		try
		{
			_exec(*it);
		}
		catch (ResponseException &e)
		{
			_client.write(e.response());
			if (dynamic_cast<AuthException *>(&e))
				throw std::runtime_error(e.response().str());
		}
		
	}
}

void Command::_exec(const Request::Body &body)
{
	map_t::iterator it = _commands.find(utils::toupper(body.first));
	if (it != _commands.end())
	{
		if (!_client.is_auth())
		{
			if (it->first != "PASS" && it->first != "NICK" && it->first != "USER" && it->first != "CAP" && it->first != "QUIT")
			{
				_client.write(ERR_NOTREGISTERED(_client.get_key("NICKNAME")));
				ERROR("Commande non autorisée: " << it->first);
				return;
			}
		}

		Payload p(_client, body);

		it->second(p);

		if (_client.is_auth() && _client.get_state() == IS_NOT_AUTH)
		{
			SUCCESS("Connexion réussie");
			say_hello(_client);
			_client.set_state(IS_AUTH);
		}
		return;
	}
	throw ResponseException(ERR_UNKNOWNCOMMAND(body.first));
}

Command::map_t Command::init_cmd()
{
	map_t map;

	map["NICK"] = &Command::nick;
	map["PASS"] = &Command::pass;
	map["USER"] = &Command::user;
	map["HELP"] = &Command::help;
	map["INFO"] = &Command::info;
	map["PRIVMSG"] = &Command::privmsg;
	map["NOTICE"] = &Command::notice;
	map["NAMES"] = &Command::names;

	// CHANNEL COMMANDS
	map["JOIN"] = &Command::join;
	map["PART"] = &Command::part;
	map["MODE"] = &Command::mode;
	map["LIST"] = &Command::list;
	map["WHO"] = &Command::who;
	map["TOPIC"] = &Command::topic;
	map["INVITE"] = &Command::invite;
	map["KICK"] = &Command::kick;

	// SHORT COMMANDS
	map["QUIT"] = &Command::quit;
	map["WHOIS"] = &Command::whois;
	map["PING"] = &Command::ping;
	map["PONG"] = &Command::pong;
	map["USERHOST"] = &Command::userhost;
	map["LUSERS"] = &Command::lusers;
	map["CAP"] = &Command::cap;


	return (map);
}

Command::map_t Command::_commands = Command::init_cmd();
