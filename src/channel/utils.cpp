#include "channel.hpp"


std::string fix_channel_name(const std::string& name)
{
	if (name[0] != '#' && name[0] != '&')
		return "#" + name;
	return name;
}