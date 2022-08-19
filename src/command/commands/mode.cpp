#include "../command.hpp"

// spntimkl, ov  channel mode, user mode

bool is_channel_mode(char c)
{
	if (
		c == 's' ||
		c == 'p' ||
		c == 'n' ||
		c == 't' ||
		c == 'i' ||
		c == 'm' ||
		c == 'k' ||
		c == 'l' ||
		c == 'b'
	)
		return true;
	return false;
}

bool is_user_mode(char c)
{
	if (
		c == 'o' ||
		c == 'v'
	)
		return true;
	return false;
}

char  find_invalid_mode(std::string str, bool (*f)(char c))
{
	if (!str.size())
		return '$';
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
		if (!f(*it))
			return *it;
	return '\0';
}

bool is_mode(char c)
{
	if (is_channel_mode(c) || is_user_mode(c))
		return true;
	return false;
}

void insert_mode(const char c, std::string& mode, std::map<char, int>& parameter_order)
{
	if (c == 'k' || c == 'l')
		parameter_order[c] = parameter_order.size();
	mode.push_back(c);
}

std::map<char, int> parse_mode(const Client& client, const std::string &modes, std::string &to_add, std::string &to_remove)
{
	(void)client;
	std::map<char, int> parameter_order;
	
	for (size_t i = 0; i < modes.size(); i++)
	{

		if (modes[i] != '+' && modes[i] != '-' && !is_mode(modes[i]))
			throw ResponseException(ERR_UNKNOWNMODE(client.get_key("NICKNAME"), modes[i]));

		if (modes[i] == '+')
		{
			while (is_mode(modes[++i]))
				insert_mode(modes[i], to_add, parameter_order);
			i--;
		}
		if (modes[i] == '-')
		{
			while (is_mode(modes[++i]))
				insert_mode(modes[i], to_remove, parameter_order);
			i--;
		}
	}
	return parameter_order;
}

void set_channel_mode(Client& client, Channel &channel, const std::string &to_add, const std::string &to_remove, const std::vector<std::string>& args, const std::map<char, int>& parameter_order)
{
	channel.give(to_add);
	channel.take(to_remove);

	if (channel.has('k') && to_add.find('k') != std::string::npos)
	{
		size_t order = parameter_order.find('k')->second;
		if (args.size() < 2 + (order + 1))
			throw ResponseException(ERR_NEEDMOREPARAMS("*", "MODE"));
		channel.set_password(args[2 + order]);
	}

	if (channel.has('l') && to_add.find('l') != std::string::npos)
	{
		size_t order = parameter_order.find('l')->second;
		if (args.size() < 2 + (order + 1))
			throw ResponseException(ERR_NEEDMOREPARAMS("*", "MODE"));
		channel.set_limit(atoi(args[2 + order].c_str()));
	}

	Channel::Dispatcher dispatcher = channel.create_dispatcher();

	std::string msg = (to_add.size() > 0 ? "+" + to_add : "") + (to_remove.size() > 0 ? "-" + to_remove : "");
	for (size_t i = 0; i < parameter_order.size(); i++)
		msg += " " + args[2 + i];

	dispatcher.load(RPL_MODE(client.fullname(), channel.get_name(), msg));
	dispatcher.send();
}

void Command::mode(Payload p)
{
	(void)p;
	if (p.body.second.size() < 1)
		throw ResponseException(ERR_NEEDMOREPARAMS("*", "MODE"));

	std::string target = fix_channel_name(p.body.second[0]);
	std::map<std::string, Channel>::iterator channel_it = Server::channels.find(target);
	if (channel_it == Server::channels.end())
		throw ResponseException(ERR_NOSUCHCHANNEL(p.client.get_key("NICKNAME"), target));

	if (p.body.second.size() < 2)
		throw ResponseException(ERR_NEEDMOREPARAMS("*", "MODE"));

	std::string modes = p.body.second[1];
	std::string to_add;
	std::string to_remove;

	std::map<char, int> parameter_order = parse_mode(p.client, modes, to_add, to_remove);

	

	// if (!find_invalid_mode(sum_mode, is_channel_mode))
	// 	set_channel_mode(p.client, channel_it->second, to_add, to_remove, p.body.second, parameter_order);
	// else if (!find_invalid_mode(sum_mode, is_user_mode))
	// 	DEBUG("USER MODE");
	// else
	// 	throw ResponseException(ERR_UNKNOWNMODE(p.client.get_key("NICKNAME"), modes));

	// DEBUG(modes);
	// DEBUG(to_add);
	// DEBUG(to_remove);
}
