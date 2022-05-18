/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/18 09:52:09 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/18 16:18:41 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

void	Client::printPara(void)
{
	std::cout << "Client : " << std::endl;
	std::cout << "\tfd       : " << this->_fd << std::endl;
	std::cout << "\tchannel  : " << this->_channel << std::endl;
	std::cout << "\tidentify : " << this->_identify << std::endl;
	std::cout << "\tusername : " << this->_username << std::endl;
	std::cout << "\tnickname : " << this->_nickname << std::endl;
}