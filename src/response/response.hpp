/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:25 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/27 08:43:02 by mbonnet          ###   ########.fr       */
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
# define ERR_INVITEONLYCHAN(USER) RESPONSE("443", USER, "Vous devais etre inviter pour accede a ce canal")
# define ERR_CHANNELISFULL(USER) RESPONSE("444", USER, "Ce channel est plain") 
# define ERR_NOSUCHCHANNEL(USER) RESPONSE("445", USER, "Aucun channel de ce type") 
# define ERR_BANNEDFROMCHAN(USER) RESPONSE("446", USER, "Vous etes banie de ce channel")
# define ERR_BADCHANNELKEY(USER) RESPONSE("447", USER, "Vous n avais pas le bon code")
# define ERR_TOOMANYCHANNELS(USER) RESPONSE("448", USER, "Vous etes deja connecter a se channel") 
# define ERR_BADCHANMASK(USER) RESPONSE("449", USER, "Mauvais masque")
# define RPL_TOPIC(USER) RESPONSE("310", USER, "Vous etes bien connecter au canal")
# define RPL_NAMREPLY(USER) RESPONSE("311", USER, "Bien venue dans ce cannal")

class Response
{
	public:
		Response(std::string os);
		std::string str() const;
	private:
		std::string _str;
};

class ResponseException : public std::exception
{
public:
	ResponseException(Response res);
	~ResponseException() throw();
	virtual const char *what() const throw();
	Response &response();
protected:
	Response _res;
};

class AuthException : public ResponseException
{
public:
	AuthException(Response res);
};


#endif