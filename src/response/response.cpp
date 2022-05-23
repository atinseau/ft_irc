/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:29 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 12:10:30 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response(std::string os): _str(os) {}

std::string Response::str() const
{
	return (_str);
}