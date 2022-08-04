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

std::vector<std::string> split(const char *str, char c);
std::string join(const std::vector<std::string>& vec, const char* sep, size_t offset = 0);
std::string format_time(struct timeval& tv);


template< class Container, class Function>
void for_each(Container container, Function fn)
{
	for (typename Container::iterator it = container.begin(); it != container.end(); it++)
		fn(it, container);
}

template <class Container, typename EraseType>
void remove_if(Container& vec, const EraseType value)
{
	vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
}

#endif 