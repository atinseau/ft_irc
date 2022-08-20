 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:25 by mbonnet           #+#    #+#             */
/*   Updated: 2022/06/03 19:46:47 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include <string>

# define HOST "localhost"
# define F(x) std::string("") + x + "\n"

# define RPL_WELCOME(NICK)						F(":" + HOST + " 001 " + NICK + " :Bienvenur sur le server FT_IRC " + NICK)
# define RPL_YOURHOST(NICK)						F(":" + HOST + " 002 " + NICK + " :Votre host est " + HOST)
# define RPL_CREATED(NICK, TIME)				F(":" + HOST + " 003 " + NICK + " :Le server a été créé le " + TIME)
# define RPL_INFO(NICK, MSG)					F(":" + HOST + " 371 " + NICK + " :" + MSG)
# define RPL_NICKCHANGE(OLD, NEW)				F(":" + OLD + " NICK " + ":" + NEW)

# define RPL_PART(FULLNAME, CHANNEL)			F(FULLNAME + " PART " + CHANNEL)
# define RPL_PRIVMSG(FULLNAME, TO, MSG)			F(FULLNAME + " PRIVMSG " + TO + " :" + MSG)
# define RPL_JOIN(FULLNAME, CHANNEL)			F(FULLNAME + " JOIN " + CHANNEL)
# define RPL_MODE(FULLNAME, CHANNEL, MODE)		F(FULLNAME + " MODE " + CHANNEL + " " + MODE)

# define RPL_NAMREPLY(NICK, CHANNEL, LIST)		F(":" + HOST + " 353 " + NICK + " = " + CHANNEL + " :" + LIST)
# define RPL_ENDOFNAMES(NICK, CHANNEL)			F(":" + HOST + " 366 " + NICK + " " + CHANNEL + " :End of /NAMES list")
# define RPL_LISTSTART(NICK)					F(":" + HOST + " 321 " + NICK + " Channel :Liste des channels")
# define RPL_LIST(NICK, CHANNEL, COUNT, TOPIC)	F(":" + HOST + " 322 " + NICK + " " + CHANNEL + " " + COUNT + " : " + TOPIC)
# define RPL_LISTEND(NICK)						F(":" + HOST + " 323 " + NICK + " :End of /LIST")
# define RPL_TOPIC(NICK, CHANNEL, TOPIC)		F(":" + HOST + " 332 " + NICK + " " + CHANNEL + " :" + TOPIC)
# define RPL_NOTOPIC(NICK, CHANNEL)				F(":" + HOST + " 331 " + NICK + " " + CHANNEL + " :Le channel n'a pas de topic")

# define ERR_UNKNOWNCOMMAND(COMMAND)			F(":" + HOST + " 421 * :La commande " + COMMAND + " n'existe pas")
# define ERR_NONICKNAMEGIVEN					F(":" + HOST + " 431 :Aucun surnom d'utilisateur n'a etait fourni")
# define ERR_ERRONEUSNICKNAME(NICK)				F(":" + HOST + " 432 " + NICK + " :Surnom d'utilisateur invalide")
# define ERR_NICKNAMEINUSE(NICK)				F(":" + HOST + " 433 " + NICK + " :Surnom d'utilisateur déjà utilisé")
# define ERR_ALREADYREGISTRED(NICK)				F(":" + HOST + " 462 " + NICK + " :Vous êtes déjà enregistré")
# define ERR_NEEDMOREPARAMS(NICK, COMMAND)		F(":" + HOST + " 461 " + NICK + " " + COMMAND + " :Paramètres manquants")
# define ERR_INVALIDPASS						F(":" + HOST + " 482 * :Mot de passe invalide")
# define ERR_NOTONCHANNEL(NICK, CHANNEL)		F(":" + HOST + " 442 " + NICK + " :Vous n'est pas dans le channel " + CHANNEL)
# define ERR_NOSUCHNICK(NICK, TO)				F(":" + HOST + " 401 " + NICK + " :Il n'y a pas d'utilisateur avec le surnom " + TO)
# define ERR_BADCHANNELKEY(NICK, CHANNEL)		F(":" + HOST + " 475 " + NICK + " :Le mot de passe du channel " + CHANNEL + " est incorrect")
# define ERR_USERONCHANNEL(NICK, CHANNEL)		F(":" + HOST + " 443 " + NICK + " :Vous êtes déjà dans le channel " + CHANNEL)
# define ERR_NOSUCHCHANNEL(NICK, CHANNEL)		F(":" + HOST + " 403 " + NICK + " :Le channel " + CHANNEL + " n'existe pas")
# define ERR_NOCHANNELS(NICK)					F(":" + HOST + " 442 " + NICK + " :Aucun channel n'est disponible")

# define ERR_UNKNOWNMODE(NICK, MODE)			F(":" + HOST + " 472 " + NICK + " :Mode inconnu " + MODE)
# define ERR_MODESYNTAX(NICK)					F(":" + HOST + " 472 " + NICK + " :Syntaxe pour le mode du channel invalide")
# define ERR_DUPLICATEMODE(NICK, MODE)			F(":" + HOST + " 472 " + NICK + " :Syntaxe erreur, le mode " + MODE + " est present plusieurs fois")
# define ERR_NOMODE(NICK, CHANNEL)				F(":" + HOST + " 472 " + NICK + " :Aucun mode pour " + CHANNEL + " n'a été fourni")
# define ERR_AMBIGUOUS(NICK, MODE, CHANNEL)				F(":" + HOST + " 472 " + NICK + " :Attention, le mode " + MODE + " est ambigu pour le channel " + CHANNEL)

# define ERR_INVITEONLYCHAN(NICK, CHANNEL)		F(":" + HOST + " 473 " + NICK + " :Le channel " + CHANNEL + " est sur invitation uniquement (+i)")
# define ERR_CHANOPRIVSNEEDED(NICK, CHANNEL)	F(":" + HOST + " 482 " + NICK + " :Vous n'êtes pas opérateur du channel " + CHANNEL + " (+o )")


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
	ResponseException(std::string response);
	~ResponseException() throw();
	virtual const char *what() const throw();
	Response &response();
protected:
	Response _res;
};

class AuthException : public ResponseException
{
public:
	AuthException(std::string response);
};


#endif