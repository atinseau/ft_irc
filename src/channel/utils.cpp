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