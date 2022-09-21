#include "response.hpp"

Response::Response(std::string os): _str(os) {}

std::string Response::str() const
{
	return (_str);
}

ResponseException::ResponseException(std::string response) : _res(Response(response)){}

ResponseException::~ResponseException() throw(){}

const char *ResponseException::what() const throw()
{
	return "ResponseException";
}

Response& ResponseException::response()
{
	return (_res);
}

AuthException::AuthException(std::string response): ResponseException(response) {}