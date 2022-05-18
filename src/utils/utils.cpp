/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:23 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 09:52:24 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"


strvec split(const char *str, char c)
{
	strvec res;

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