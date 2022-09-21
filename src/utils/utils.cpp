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

size_t find_list(std::string &str, const char *sep)
{
	int i = 0;
	size_t pos = 0;
	while (sep[i] != '\0')
	{
		if ((pos = str.find(sep[i])) != std::string::npos)
			return pos;
		i++;
	}
	return std::string::npos;
}

namespace utils
{
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

	std::string toupper(const std::string &str)
	{
		std::string res;
		for (size_t i = 0; i < str.size(); i++)
			res += std::toupper(str[i]);
		return res;
	}

	std::vector<std::string> split(const char *str, const char *sep)
	{
		std::vector<std::string> res;
		std::string dup;
		size_t pos = 0;

		if (!str)
			return res;
		dup.append(str);
		while ((pos = find_list(dup, sep)) != std::string::npos)
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

	std::string format_time(struct timeval &tv)
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

	size_t strlen(const char *str)
	{
		size_t i = 0;
		while (str[i])
			i++;
		return (i);
	}

	int atoi(const char *str)
	{
		int res = 0;
		int sign = 1;
		if (*str == '-')
		{
			sign = -1;
			str++;
		}
		while (*str)
			res = res * 10 + (*str++ - '0');
		return (res * sign);
	}

	std::string itoa(int i)
	{
		std::string res;
		std::string reverse;
		int e;

		if (i == 0)
			return ("0");

		if (i < 0)
		{
			reverse += '-';
			i *= -1;
		}
		while (i)
		{
			res += i % 10 + '0';
			i /= 10;
		}
		e = res.size();
		while (e--)
			reverse += res[e];
		return reverse;
	}

	void pop_back(std::string &str)
	{
		str.erase(str.size() - 1);
	}

	void get_time_lock()
	{
		static bool lock = false;
		static struct timeval time;
		struct timeval now;

		if (!lock)
		{
			gettimeofday(&time, NULL);
			lock = true;
			return;
		}
		gettimeofday(&now, NULL);

		long long diff = (now.tv_sec * 1000000 + now.tv_usec) - (time.tv_sec * 1000000 + time.tv_usec);
		std::cout << std::setprecision(10) << std::fixed << (float)diff / 1000000 << "s" << std::endl;
		lock = false;
	}
};