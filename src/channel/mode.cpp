#include "channel.hpp"


Mode::Mode(): _modes() {}

void Mode::give(std::string modes)
{
	size_t i = 0;
	while (i < modes.size())
	{
		if (utils::find(_modes.begin(), _modes.end(), modes[i]) == _modes.end())
			_modes.push_back(modes[i]);
		i++;
	}
}

void Mode::take(std::string modes)
{
	size_t i = 0;
	while (i < modes.size())
	{
		utils::remove_if(_modes, modes[i]);
		i++;
	}
}

const std::vector<char>& Mode::get_modes() const
{
	return _modes;
}


bool Mode::has(char mode) const
{
	return utils::find(_modes.begin(), _modes.end(), mode) != _modes.end();
}
