/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:25 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/26 08:24:55 by mbonnet          ###   ########.fr       */
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

//JOIN
# define ERR_INVITEONLYCHAN(USER) RESPONSE("0", USER, "Vous devais etre inviter pour accede a ce canal")
# define ERR_CHANNELISFULL(USER) RESPONSE("0", USER, "Ce channel est plain") 
# define ERR_NOSUCHCHANNEL(USER) RESPONSE("0", USER, "Aucun channel de ce type") 
# define ERR_BANNEDFROMCHAN(USER) RESPONSE("0", USER, "Vous etes banie de ce channel")
# define ERR_BADCHANNELKEY(USER) RESPONSE("0", USER, "Vous n avais pas le bon code")
# define ERR_TOOMANYCHANNELS(USER) RESPONSE("0", USER, "Vous etes deja connecter a se channel") 
# define ERR_BADCHANMASK(USER) RESPONSE("0", USER, "Mauvais masque")
//# define RPL_TOPIC(USER) RESPONSE("0", USER, "Bien venue dans ce cannal")
//# define RPL_NAMREPLY(USER) RESPONSE("0", USER, "Bien venue dans ce cannal")






class Response
{
	public:
		Response(std::string os);
		std::string str() const;
	private:
		std::string _str;
};

#endif