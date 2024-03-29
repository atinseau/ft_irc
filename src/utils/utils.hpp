#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <iomanip>

namespace utils
{

	std::vector<std::string> split(const char *str, char c);
	std::vector<std::string> split(const char *str, const char* sep);
	std::string join(const std::vector<std::string> &vec, const char *sep, size_t offset = 0);
	std::string format_time(struct timeval &tv);
	size_t strlen(const char *str);
	int atoi(const char *str);
	std::string itoa(int n);
	void pop_back(std::string &str);
	void get_time_lock();

	std::string toupper(const std::string &str);

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