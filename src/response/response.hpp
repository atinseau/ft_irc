/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:25 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/23 19:55:53 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include <string>

# define HOST "localhost"

# define RESPONSE(CODE, USER, MSG) std::string(":") + HOST + " " + CODE + " " + USER + " :" + MSG + "\r\n"

// PASS
# define ERR_NEEDMOREPARAMS(USER) RESPONSE("461", USER, "Argument(s) manquant(s)")
# define ERR_ALREADYREGISTRED(USER) RESPONSE("462", USER, "Vous êtes déjà enregistré")

// NICK
# define ERR_NONICKNAMEGIVEN RESPONSE("431", "*", "Aucun pseudo n'a été donné")
# define ERR_NICKNAMEINUSE(USER) RESPONSE("433", USER, "Pseudo déjà utilisé")
# define ERR_ERRONEUSNICKNAME(USER) RESPONSE("432", USER, "Pseudo invalide (contient des caractères invalides ou trop long)")
# define ERR_PASSCHANNEL(USER) RESPONSE("0", USER, "Le mot de passe est incorrecte")
# define ERR_CHANNELDOESNOTEXIST(USER) RESPONSE("0", USER, "Le channel rechercher n existe pas")
# define ERR_CHANNELISNOTAVAILABLE(USER) RESPONSE("0", USER, "Le channel n est pas disponible")

class Response
{
	public:
		Response(std::string os);
		std::string str() const;
	private:
		std::string _str;
};

#endif