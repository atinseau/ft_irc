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
	std::string dup;
	size_t pos = 0;

	if (!str)
		return res;
	dup.append(str);
	while ((pos = dup.find(c)) != std::string::npos)
	{
		res.push_back(dup.substr(0, pos));
		dup.erase(0, pos + 1);
	}
	res.push_back(dup);
	return res;
}

std::string join(const std::vector<std::string> &vec, const char *sep, size_t offset)
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

std::string format_time(struct timeval& tv)
{
	time_t t;
	struct tm *info;
	char buffer[64];
	t = tv.tv_sec;
	info = localtime(&t);
	strftime(buffer, sizeof buffer, "%I:%M %p.", info);
	char *date = asctime(info);
	date[strlen(date) - 1] = '\0';
	return (std::string(date) + " à " + buffer);
}