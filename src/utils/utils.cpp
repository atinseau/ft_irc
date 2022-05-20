/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:23 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 15:09:55 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool whitesapece(char c)
{
	if (c <= 32 && c >= 28)
		return (true);
	return (false);
}

std::vector<std::string> split(const char *str, char c)
{
	std::vector<std::string> res;

	int i = 0;
	int cursor = 0;
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