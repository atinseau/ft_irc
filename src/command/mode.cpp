/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 16:43:14 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/24 17:06:46 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

// bool Command::mode_o(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_p(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_s(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_i(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_t(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_n(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_m(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_l(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_b(Channel channel, std::vector<std::string> cmd)
// {}
// bool Command::mode_v(Channel channel, std::vector<std::string> cmd)
// {}
bool Command::mode_k(Channel channel, std::vector<std::string> cmd)
{
	if (channel.get_password() == "\0")
		return (true);
	for (std::vector<std::string>::iterator it = cmd.begin(); it != cmd.end(); it++)
		if (*it == channel.get_password())
			return (true);
	return (false);
}

std::map<char, Command::func_t2> Command::init_mode()
{
	std::map<char, Command::func_t2> map;

	// map['o'] = &Command::mode_o;
	// map['p'] = &Command::mode_p;
	// map['s'] = &Command::mode_s;
	// map['i'] = &Command::mode_i;
	// map['t'] = &Command::mode_t;
	// map['n'] = &Command::mode_n;
	// map['m'] = &Command::mode_m;
	// map['l'] = &Command::mode_l;
	// map['b'] = &Command::mode_b;
	// map['v'] = &Command::mode_v;
	map['k'] = &Command::mode_k;

	return (map);
}
std::map<char, Command::func_t2> Command::_mode = Command::init_mode();