/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:23 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 08:06:48 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> split(const char *str, char c)
{
	std::vector<std::string> res;
	int i = 0;
	int cursor = 0;

	if (!str)
		return (res);

	while(1)
	{
		if (str[i] == c || str[i] == '\0')
		{
			std::string n(str + cursor, i - cursor);
			res.push_back(n);
			if (str[i] == '\0')
				break;
			cursor += n.length() + 1;
		}
		i++;
	}
	return res;
}

std::string join(const std::vector<std::string>& vec, const char* sep, size_t offset)
{
	std::string res;
	for (size_t i = offset; i < vec.size(); i++)
	{
		res += vec[i];
		if (i != vec.size() - 1)
			res += sep;
	}
	return (res);
}