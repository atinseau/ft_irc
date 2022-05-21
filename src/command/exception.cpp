#include "command.hpp"

Command::Payload::Payload(Client &client, const std::vector<Client>& clients, const Request::Body& body): client(client), clients(clients), body(body) {}

Command::ResponseException::ResponseException(Response res) : _res(res){}

Command::ResponseException::~ResponseException() throw(){}

const char *Command::ResponseException::what() const throw()
{
	return "ResponseException";
}

Response& Command::ResponseException::response()
{
	return (_res);
}


Command::AuthException::AuthException(Response res) : ResponseException(res) {}