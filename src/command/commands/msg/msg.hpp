#ifndef MSG_HPP

#include "../../command.hpp"

struct Recipient
{
	std::string identifier;
	bool is_channel;
};

std::vector<Recipient> get_recipients(const std::string &payload);
std::string get_message(const std::vector<std::string> &vec);

#endif