/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:26 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/02 08:07:50 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <iomanip>


std::vector<std::string> split(const char *str, char c);
std::string join(const std::vector<std::string> &vec, const char *sep, size_t offset = 0);
std::string format_time(struct timeval &tv);
size_t strlen(const char *str);
int atoi(const char *str);
std::string itoa(int n);
void pop_back(std::string &str);
void get_time_lock();

namespace utils
{

	template <typename T, typename U>
	T find(T begin, T end, U value)
	{
		while (begin != end)
		{
			if (*begin == value)
				return begin;
			begin++;
		}
		return end;
	}

	template <class Container, class Function>
	void for_each(Container container, Function fn)
	{
		for (typename Container::iterator it = container.begin(); it != container.end(); it++)
			fn(it, container);
	}

	template <class Container, typename EraseType>
	void remove_if(Container &vec, const EraseType value)
	{
		typename Container::iterator it = utils::find(vec.begin(), vec.end(), value);
		if (it != vec.end())
			vec.erase(it);
	}

};

#endif