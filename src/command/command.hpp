#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../irc.hpp"

class Command
{
	struct Payload
	{
		Payload(Client& client, const Request::Body& body);

		Client& client;
		const Request::Body& body;
	};

	typedef void (*func_t)(Payload&);
	typedef std::map<std::string, func_t> map_t;
public:


	Command(Client& client);

	void exec(const std::vector<Request::Body>& bodies);

	// STANDALONE COMMANDS
	static void nick(Payload& p);
	static void pass(Payload& p);
	static void user(Payload& p);
	static void help(Payload& p);
	static void info(Payload& p);
	static void privmsg(Payload& p);
	static void quit(Payload& p);
	static void whois(Payload& p);
	static void ping(Payload& p);
	static void notice(Payload& p);
	static void pong(Payload& p);
	static void userhost(Payload& p);
	static void lusers(Payload& p);
	static void cap(Payload& p);
	static void names(Payload& payload);

	// CHANNEL COMMANDS
	static void join(Payload& p);
	static void part(Payload& p);
	static void mode(Payload& p);
	static void topic(Payload& p);
	static void list(Payload& p);
	static void who(Payload& p);
	static void invite(Payload& p);
	static void kick(Payload& p);

private:
	static map_t init_cmd();
	static map_t _commands;

	Client& _client;

	void _exec(const Request::Body& body);
};

void say_hello(Client& client);

#endif