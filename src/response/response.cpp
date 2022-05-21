#include "response.hpp"

Response::Response(std::ostringstream os): _str(os.str()) {}

std::string Response::str() const
{
	return (_str);
}