#include "channel.hpp"


std::string fix_channel_name(const std::string& name)
{
	if (name[0] != '#' && name[0] != '&')
		return "#" + name;
	return name;
}

bool operator_is(const Operator& op, char p)
{
	const std::vector<char>& modes = op.get_modes();

	std::vector<char>::const_iterator it = utils::find(modes.begin(), modes.end(), p);
	if (it == modes.end())
		return false;
	return true;
}


bool is_valid_mask(const char *mask)
{
	if (!mask)
		return false;

	std::vector<char> tokens;
	char sep[] = {'!', '@'};

	for (int i = 0; i < mask[i]; i++)
	{
		for (int e = 0; e < 2; e++)
		{
			if (mask[i] == sep[e])
			{
				if (mask[i + 1] && (mask[i + 1] == sep[0] || mask[i + 1] == sep[1]))
					return false;
				if (mask[i + 1] == '\0')
					return false;
				if (utils::find(tokens.begin(), tokens.end(), sep[e]) != tokens.end())
					return false;
				tokens.push_back(sep[e]);
			}
		}
	}

	if (tokens.size() == 0)
		return true;
	if (tokens.size() != 2 || (tokens.size() == 2 && (tokens[0] != '!' && tokens[1] != '@')))
		return false;
	return true;
}


bool string_mask_match(const std::string &mask, const std::string &value)
{
	std::vector<std::string> tokens = utils::split(mask.c_str(), "*");
	size_t i = 0;

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		if (it->empty())
			continue;
		size_t next_pos = value.find(*it, i);
		if (next_pos == std::string::npos || (mask.find(*it) + it->size() == mask.size() && value[next_pos + it->size()] != '\0'))
			return false;
		i = next_pos + it->size();
	}
	return true;
}


bool user_match_mask(const std::string &mask, const Client& client)
{
	std::vector<std::string> tokens = utils::split(mask.c_str(), "!@");
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		if (it - tokens.begin() == 0)
		{
			if (!string_mask_match(*it, client.get_key("NICKNAME")))
				return false;
		}
		else if (it - tokens.begin() == 1)
		{
			if (!string_mask_match(*it, client.get_key("USERNAME")))
				return false;
		}
		else if (it - tokens.begin() == 2)
		{
			if (!string_mask_match(*it, client.get_key("HOSTNAME")))
				return false;
		}
	}
	return true;
}