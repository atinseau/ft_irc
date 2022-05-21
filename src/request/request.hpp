#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../utils/utils.hpp"

typedef std::pair<int, std::string> request_t;

class Request : public request_t
{

public:
	typedef std::pair<std::string, std::vector<std::string> > Body;
	enum Type
	{
		QUIT,
		NONE,
		ERROR,
		SUCCESS
	};

	Request();
	explicit Request(int n, char *s);

	request_t::first_type size() const;

	Type type() const;
	Body body() const;

	void set_type(Type t);

private:
	Type _type;
};


std::ostream& operator<<(std::ostream& os, const Request::Body& body);

#endif