#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../irc.hpp"

class Command
{
	struct Payload
	{
		Payload(Client& client, const std::vector<Client>& clients, const Request::Body& body);

		Client &client;
		const std::vector<Client>& clients;
		const Request::Body &body;
	};

	typedef void (*func_t)(Payload);
	typedef std::map<std::string, func_t> map_t;

public:
	Command(Client &client, std::vector<Client> &clients);

	void operator[](const Request::Body &body);

	static void nick(Payload p);
	static void pass(Payload p);
	static void user(Payload p);
	static void join(Payload p);

	class ResponseException : public std::exception
	{
	public:
		ResponseException(Response res);
		~ResponseException() throw();

		virtual const char *what() const throw();
		Response &response();

	protected:
		Response _res;
	};

	class AuthException : public ResponseException
	{
	public:
		AuthException(Response res);
	};

private:
	static map_t init();
	static map_t _commands;

	Client &_client;
	std::vector<Client> &_clients;
};

#endif