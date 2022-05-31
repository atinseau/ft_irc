/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:32 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/31 11:49:17 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

Command::Payload::Payload(Client &client, std::map<int,Client> &clients, const Request::Body& body, std::map<std::string, Channel>& channels) : client(client), clients(clients), body(body), channels(channels) {}
