#include "msg.hpp"


std::vector<Recipient> get_recipients(const std::string &payload)
{
	std::vector<Recipient> recipients;

	std::vector<std::string> tokens = utils::split(payload.c_str(), ',');

	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		Recipient recipient;
		if (it->at(0) == '#' || it->at(0) == '&')
			recipient.is_channel = true;
		else
			recipient.is_channel = false;
		recipient.identifier = *it;
		recipients.push_back(recipient);
	}
	return (recipients);
}

std::string get_message(const std::vector<std::string> &vec)
{
	std::string message = utils::join(vec, " ", 1);
	if (message[0] == ':')
		message.erase(0, 1);
	return (message);
}