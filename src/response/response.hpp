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

# define RESPONSE(CODE, USER, MSG) std::string(":") + HOST + " " + CODE + " " + USER + " :" + MSG + "\r\n"

// REGISTRATION //
# define RPL_WELCOME(USER) RESPONSE("001", USER, "Bienvenue sur le serveur FT_IRC de 42")
# define RPL_YOURHOST(USER) RESPONSE("002", USER, "Votre hostname est " + HOST)
# define RPL_CREATED(USER, DATE) RESPONSE("003", USER, "Le serveur a été créé le " + DATE)

// NICK //
# define ERR_NONICKNAMEGIVEN RESPONSE("431", "*", "Aucun surnom d'utilisateur donné")
# define ERR_NICKNAMEINUSE(NICK) RESPONSE("432", "*", "Le surnom d'utilisateur " + NICK + " déjà utilisé")
# define ERR_ERRONEUSNICKNAME(NICK) RESPONSE("433", "*", "Le surnom d'utilisateur " + NICK + " n'est pas valide")

// USER //

// PASSWORD //
# define ERR_INVALIDPASS RESPONSE("482", "*", "Mot de passe invalide")

// INFO //
# define RPL_INFO(USER, INFO) RESPONSE("371", USER, INFO)

// PRIVMSG //
# define ERR_NOSUCHNICK(NICK, RECIPIENT) RESPONSE("401", NICK, "Aucun utilisateur avec le surnom " + RECIPIENT + " n'a été trouvé")
# define RPL_PRIVMSG(FROM, TO, MESSAGE) std::string(":") + FROM + "!" + FROM + "@" + HOST + " PRIVMSG " + TO + " :" + MESSAGE + "\r\n"



// JOIN //
# define USER_JOIN_ITEM(NICK) NICK + "!" + NICK + "@" + HOST

# define ERR_BADCHANNELKEY(CHANNEL, USER) RESPONSE("475", USER, "Le mot de passe du channel " + CHANNEL + " est incorrect")
# define ERR_NOSUCHCHANNEL(CHANNEL, USER) RESPONSE("403", USER, "Impossible de rejoindre ou créer ce channel " + CHANNEL + " , nom de channel incorrect")
# define ERR_USERONCHANNEL(CHANNEL, USER) RESPONSE("443", USER, "Vous êtes déjà dans le channel " + CHANNEL)
# define RPL_JOIN(USER, CHANNEL) std::string(":") + USER + "!" + USER + "@" + HOST + " JOIN :" + CHANNEL + "\r\n"
# define RPL_JOINLIST(USER, CHANNEL, USERS_LIST) RESPONSE("353", USER + " = " + CHANNEL, USERS_LIST)
# define RPL_ENDOFJOINLIST(USER, CHANNEL) RESPONSE("366", USER + " " + CHANNEL, "Fin de la liste des utilisateurs du channel " + CHANNEL)


// std::string(":") 353 thomas = #salut :thomas!thomas@Chat4all-F15B3CF4.subs.proxad.net @arthur!arthur@Chat4all-F15B3CF4.subs.proxad.net 
// :eu3.chat4all.org 366 thomas #salut :End of /NAMES list.
// LIST //

# define RPL_LISTSTART(USER) RESPONSE("321", USER, "Liste des channels disponibles :")
# define RPL_LIST(USER, CHANNEL, USERS, TOPIC) RESPONSE("322", USER + " " + CHANNEL + " " + USERS, TOPIC)
# define RPL_LISTEND(USER) RESPONSE("323", USER, "Fin de la liste des channels")
# define ERR_NOCHANNELS(USER) RESPONSE("442", USER, "Aucun channel disponible")

// PART //
# define RPL_PART(USER, CHANNEL) RESPONSE("442", USER, "Vous avez quitté le channel " + CHANNEL)
# define ERR_NOTONCHANNEL(CHANNEL, USER) RESPONSE("442", USER, "Vous n'êtes pas dans le channel " + CHANNEL)

// WHOIS //
# define RPL_ENDOFWHOIS(USER) RESPONSE("318", USER, "Fin de whois")
# define RPL_USERHOST(USER, NICK) RESPONSE("302", USER, USER + "=+" + NICK + "@" + HOST)

// PING //
# define RPL_PING(USER) RESPONSE("PONG", USER, "Pong")
# define RPL_PONG(NAME) RESPONSE("PING", NAME, "Ping")

// LUSERS //
# define RPL_LUSERCLIENT(USER) RESPONSE("251", USER, "Utilisateurs connectés : " + itoa(Server::clients.size()))

// GLOBAL //
# define ERR_ALREADYREGISTRED(USER) RESPONSE("462", USER, "Vous etes déjà enregistré")
# define ERR_NEEDMOREPARAMS(USER, CMD) RESPONSE("461", USER, "La commande " + CMD + " requiert plus de paramètres")
# define ERR_UNKNOWNCOMMAND(CMD) RESPONSE("421", "*", "Commande inconnue : " + CMD)


//a voir si se message d erreur est bien juste
# define ERR_BADCHANMASK(USER) RESPONSE("447", USER, "Mauvais masque")


# define ERR_NOTHISUSER(USER) RESPONSE("403", USER,"<" + USER + ">" + ":is not on this server")
# define ERR_CANNOTSENDTOCHAN(USER, CHANNEL) RESPONSE("404", USER,"<" + CHANNEL + ">" + ":Cannot send to channel")
# define ERR_TOOMANYCHANNELS(USER, CHANNEL) RESPONSE("405", USER, "<" + CHANNEL + ">" + ":You have joined too many channels")
# define ERR_WASNOSUCHNICK(USER, CHANNEL) RESPONSE("406", USER, "<" + CHANNEL + ">" + ":There was no such nickname")
# define ERR_TOOMANYTARGETS(USER, CHANNEL) RESPONSE("407", USER,"<" + CHANNEL + ">" + ":Duplicate recipients. No message delivered")
# define ERR_NOORIGIN(USER) RESPONSE("409", USER, ":No origin specified")
# define ERR_NORECIPIENT(USER, COMMANDE) RESPONSE("411", USER, ":No recipient given <" + COMMANDE + ">" )
# define ERR_NOTEXTTOSEND(USER) RESPONSE("412", USER, ":No text to send")
# define ERR_NOTOPLEVEL(USER, MASQUE) RESPONSE("413", USER, "<" + MASQUE + "> :No toplevel domain specified")
# define ERR_WILDTOPLEVEL(USER, MASQUE) RESPONSE("414", USER, "<" + MASQUE + "> :Wildcard in toplevel domain")
# define ERR_NOMOTD(USER) RESPONSE("422", USER, ":MOTD File is missing")
# define ERR_NOADMININFO(USER, SERVER) RESPONSE("423", USER, "<" + SERVER + "> :Unknown command" )
# define ERR_FILEERROR(USER, OPPERATION, FICHIER) RESPONSE("424", USER, ":File error doing <" + OPPERATION + "> on <" + FICHIER + ">")
# define ERR_NICKCOLLISION(USER, NICK) RESPONSE("436", USER, "<" + NICK + "> :Nickname collision KILL")
# define ERR_USERNOTINCHANNEL(USER, CHANNEL) RESPONSE("441", USER, "<" + USER + ">" + "<" + CHANNEL + "> :They aren't on that channel")
# define ERR_NOLOGIN(USER) RESPONSE("444", USER, "<" + USER + "> :User not logged in")
# define ERR_SUMMONDISABLED(USER) RESPONSE("445", USER, ":SUMMON has been disabled")
# define ERR_USERSDISABLED(USER) RESPONSE("446", USER, ":USERS has been disabled")
# define ERR_NOTREGISTERED(USER) RESPONSE("451", USER, ":You have not registered")
# define ERR_NOPERMFORHOST(USER) RESPONSE("463", USER, ":Your host isn't among the privileged")
# define ERR_PASSWDMISMATCH(USER) RESPONSE("464", USER, ":Password incorrect")
# define ERR_YOUREBANNEDCREEP(USER) RESPONSE("465", USER, ":You are banned from this server")
# define ERR_KEYSET(USER, CHANNEL) RESPONSE("467", USER, "<" + CHANNEL + "> :Channel key already set")
# define ERR_CHANNELISFULL(USER, CHANNEL) RESPONSE("471", USER, "<" + CHANNEL + "> :Cannot join channel (+l)")
# define ERR_UNKNOWNMODE(USER, CARACTERE) RESPONSE("472", USER, "<" + CARACTERE + "> :is unknown mode char to me")
# define ERR_INVITEONLYCHAN(USER, CHANNEL) RESPONSE("473", USER, "<" + CHANNEL + "> :Cannot join channel (+i)")
# define ERR_BANNEDFROMCHAN(USER, CHANNEL) RESPONSE("474", USER, "<" + CHANNEL + "> :Cannot join channel (+b)")
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
# define RPL_INVITING(USER) RESPONSE("370", USER, "Vaux channel au on etait modifier...")
# define RPL_MOUVMODE(USER, CHANNEL, MODE) RESPONSE("370", USER, "<" + USER + "> A changer les mode " + MODE + " sur le channel <" + CHANNEL + ">")
# define RPL_MOUVKICK(USER, CLIENT, CHANNEL) RESPONSE("371", USER, "<" + USER + "> a renvoyer " + CLIENT + " du channel <" + CHANNEL + ">")
# define RPL_MOUVPART(USER, CHANNEL) RESPONSE("372", USER, "<" + USER + "> est sorti du channal <" + CHANNEL + ">")
# define RPL_NEWCANAL(USER, CHANNEL) RESPONSE("372", USER, "<" + USER + "> est connecter au channel <" + CHANNEL + "> nouvelement construit")


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