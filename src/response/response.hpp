/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:10:25 by mbonnet           #+#    #+#             */
/*   Updated: 2022/05/31 15:21:22 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include <string>

# define HOST "localhost"

# define RESPONSE(CODE, USER, MSG) std::string(":") + HOST + " " + CODE + " " + USER + " :" + MSG + "\r\n"

//a voir si se message d erreur est bien juste
# define ERR_BADCHANMASK(USER) RESPONSE("447", USER, "Mauvais masque")



# define ERR_NOSUCHCHANNEL(USER, CHANNEL) RESPONSE("403", USER,"<" + CHANNEL + ">" + ":No such channel")
# define ERR_CANNOTSENDTOCHAN(USER, CHANNEL) RESPONSE("404", USER,"<" + CHANNEL + ">" + ":Cannot send to channel")
# define ERR_TOOMANYCHANNELS(USER, CHANNEL) RESPONSE("405", USER, "<" + CHANNEL + ">" + ":You have joined too many channels")
# define ERR_WASNOSUCHNICK(USER, CHANNEL) RESPONSE("406", USER, "<" + CHANNEL + ">" + ":There was no such nickname")
# define ERR_TOOMANYTARGETS(USER, CHANNEL) RESPONSE("407", USER,"<" + CHANNEL + ">" + ":Duplicate recipients. No message delivered")
# define ERR_NOORIGIN(USER) RESPONSE("409", USER, ":No origin specified")
# define ERR_NORECIPIENT(USER, COMMANDE) RESPONSE("411", USER, ":No recipient given <" + COMMANDE + ">" )
# define ERR_NOTEXTTOSEND(USER) RESPONSE("412", USER, ":No text to send")
# define ERR_NOTOPLEVEL(USER, MASQUE) RESPONSE("413", USER, "<" + MASQUE + "> :No toplevel domain specified")
# define ERR_WILDTOPLEVEL(USER, MASQUE) RESPONSE("414", USER, "<" + MASQUE + "> :Wildcard in toplevel domain")
# define ERR_UNKNOWNCOMMAND(USER,COMMANDE) RESPONSE("421", USER, "<" + COMMANDE + "> :Unknown command" )
# define ERR_NOMOTD(USER) RESPONSE("422", USER, ":MOTD File is missing")
# define ERR_NOADMININFO(USER, SERVER) RESPONSE("423", USER, "<" + SERVER + "> :Unknown command" )
# define ERR_FILEERROR(USER, OPPERATION, FICHIER) RESPONSE("424", USER, ":File error doing <" + OPPERATION + "> on <" + FICHIER + ">")
# define ERR_NONICKNAMEGIVEN(USER) RESPONSE("431", USER, ":No nickname given")
# define ERR_ERRONEUSNICKNAME(USER, PSEUDO) RESPONSE("432", USER, "<" + PSEUDO + "> :Erroneus nickname")
# define ERR_NICKNAMEINUSE(USER, NICK) RESPONSE("433", USER, "<" + NICK + ">  :Nickname is already in use")
# define ERR_NICKCOLLISION(USER, NICK) RESPONSE("436", USER, "<" + NICK + "> :Nickname collision KILL")
# define ERR_USERNOTINCHANNEL(USER, CHANNEL) RESPONSE("441", USER, "<" + USER + ">" + "<" + CHANNEL + "> :They aren't on that channel")
# define ERR_NOTONCHANNEL(USER, CHANNEL) RESPONSE("442", USER, "<" + CHANNEL + "> :You're not on that channel")
# define ERR_USERONCHANNEL(USER, CHANNEL) RESPONSE("443", USER, "<" + USER + ">" + "<" + CHANNEL + "> :is already on channel")
# define ERR_NOLOGIN(USER) RESPONSE("444", USER, "<" + USER + "> :User not logged in")
# define ERR_SUMMONDISABLED(USER) RESPONSE("445", USER, ":SUMMON has been disabled")
# define ERR_USERSDISABLED(USER) RESPONSE("446", USER, ":USERS has been disabled")
# define ERR_NOTREGISTERED(USER) RESPONSE("451", USER, ":You have not registered")
# define ERR_NEEDMOREPARAMS(USER, COMMANDE) RESPONSE("461", USER, "<" + COMMANDE + "> :Not enough parameters")
# define ERR_ALREADYREGISTRED(USER) RESPONSE("462", USER, ":You may not reregister")
# define ERR_NOPERMFORHOST(USER) RESPONSE("463", USER, ":Your host isn't among the privileged")
# define ERR_PASSWDMISMATCH(USER) RESPONSE("464", USER, ":Password incorrect")
# define ERR_YOUREBANNEDCREEP(USER) RESPONSE("465", USER, ":You are banned from this server")
# define ERR_KEYSET(USER, CHANNEL) RESPONSE("467", USER, "<" + CHANNEL + "> :Channel key already set")
# define ERR_CHANNELISFULL(USER, CHANNEL) RESPONSE("471", USER, "<" + CHANNEL + "> :Cannot join channel (+l)")
# define ERR_UNKNOWNMODE(USER, CARACTERE) RESPONSE("472", USER, "<" + CARACTERE + "> :is unknown mode char to me")
# define ERR_INVITEONLYCHAN(USER, CHANNEL) RESPONSE("473", USER, "<" + CHANNEL + "> :Cannot join channel (+i)")
# define ERR_BANNEDFROMCHAN(USER, CHANNEL) RESPONSE("474", USER, "<" + CHANNEL + "> :Cannot join channel (+b)")
# define ERR_BADCHANNELKEY(USER, CHANNEL) RESPONSE("475", USER, "<" + CHANNEL + "> :Cannot join channel (+k)")
# define ERR_NOPRIVILEGES(USER) RESPONSE("481", USER, ":Permission Denied- You're not an IRC operator")
# define ERR_CHANOPRIVSNEEDED(USER, CHANNEL) RESPONSE("482", USER, "<" + CHANNEL + "> :You're not channel operator")
# define ERR_CANTKILLSERVER(USER) RESPONSE("483", USER, ":You cant kill a server!")
# define ERR_NOOPERHOST(USER) RESPONSE("491", USER, ":No O-lines for your host")
# define ERR_UMODEUNKNOWNFLAG(USER) RESPONSE("501", USER, ":Unknown MODE flag")
# define ERR_USERSDONTMATCH(USER) RESPONSE("502", USER, ":Cant change mode for other users")

# define RPL_TOPIC(USER, TOPIC) RESPONSE("310", USER, "Sujet : " + TOPIC)
# define RPL_NOTOPIC(USER) RESPONSE("310", USER, "Ce canal n as pas de sujet")
# define RPL_MESSAGEEND(USER, MESSAGEEND) RESPONSE("310", USER, " " + MESSAGEEND)
# define RPL_NAMREPLY(USER, NAME) RESPONSE("310", USER, " " +  NAME)
# define RPL_ENDOFNAMES(USER) RESPONSE("310", USER, "\tfin des nom")
# define RPL_LISTSTART(USER) RESPONSE("310", USER, "\tdebut de la liste")
# define RPL_LIST(USER, NAME) RESPONSE("310", USER, " " +  NAME)
# define RPL_LISTEND(USER) RESPONSE("310", USER, "\tfin de la liste")
# define RPL_INVITING(USER) RESPONSE("370", USER, "Le client est bien viter")

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