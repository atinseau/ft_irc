# RESSOURCES

- [IBM TUTO](https://www.ibm.com/docs/en/i/7.4?topic=designs-using-poll-instead-select)
- [IRC DOCU](https://datatracker.ietf.org/doc/html/rfc1459)

# FUNCTION REQUIRED

- poll / 1 un seul




// # define RESPONSE(CODE, USER, MSG) std::string(":") + HOST + " " + CODE + " " + USER + " :" + MSG + "\r\n"
// # define CRESPONSE(RES) std::string(":") + RES + "\r\n"


// # define RPL_WELCOME(USER) RESPONSE("001", USER, "Bienvenue sur le serveur FT_IRC de 42")
// # define RPL_YOURHOST(USER) RESPONSE("002", USER, "Votre hostname est " + HOST)
// # define RPL_CREATED(USER, DATE) RESPONSE("003", USER, "Le serveur a été créé le " + DATE)


// # define ERR_NONICKNAMEGIVEN RESPONSE("431", "*", "Aucun surnom d'utilisateur donné")
// # define ERR_NICKNAMEINUSE(NICK) RESPONSE("432", "*", "Le surnom d'utilisateur " + NICK + " déjà utilisé")
// # define ERR_ERRONEUSNICKNAME(NICK) RESPONSE("433", "*", "Le surnom d'utilisateur " + NICK + " n'est pas valide")
// # define RPL_NICKNAMECHANGED(OLDNICK, NEWNICK) CRESPONSE(OLDNICK + " " + "NICK" + " :" + NEWNICK)


// # define ERR_INVALIDPASS RESPONSE("482", "*", "Mot de passe invalide")


// # define RPL_INFO(USER, INFO) RESPONSE("371", USER, INFO)


// # define ERR_NOSUCHNICK(NICK, RECIPIENT) RESPONSE("401", NICK, "Aucun utilisateur avec le surnom " + RECIPIENT + " n'a été trouvé")
// # define RPL_PRIVMSG(FROM, TO, MESSAGE) CRESPONSE(FROM + "!" + FROM + "@" + HOST + " PRIVMSG " + TO + " :" + MESSAGE)


// # define ERR_BADCHANNELKEY(USER, CHANNEL) RESPONSE("475", USER, "Le mot de passe du channel " + CHANNEL + " est incorrect")
// # define ERR_NOSUCHCHANNEL(USER, CHANNEL) RESPONSE("403", USER, "Impossible de rejoindre ou créer ce channel " + CHANNEL + " , nom de channel incorrect")
// # define ERR_USERONCHANNEL(USER, CHANNEL) RESPONSE("443", USER, "Vous êtes déjà dans le channel " + CHANNEL)
// # define RPL_JOINLIST(USER, CHANNEL, NICKNAME_LIST) RESPONSE("353", USER + " = " + CHANNEL, NICKNAME_LIST)
// # define RPL_ENDOFJOINLIST(USER, CHANNEL) RESPONSE("366", USER + " " + CHANNEL, "Fin de la liste des utilisateurs du channel " + CHANNEL)
// # define RPL_LISTSTART(USER) RESPONSE("321", USER, "Liste des channels disponibles :")
// # define RPL_LIST(USER, CHANNEL, USERS, TOPIC) RESPONSE("322", USER + " " + CHANNEL + " " + USERS, TOPIC)
// # define RPL_LISTEND(USER) RESPONSE("323", USER, "Fin de la liste des channels")
// # define ERR_NOCHANNELS(USER) RESPONSE("442", USER, "Aucun channel disponible")

// # define RPL_JOIN(USER, CHANNEL) CRESPONSE(USER + "!" + USER + "@" + HOST + " JOIN :" + CHANNEL)


// // PART //
// # define RPL_PART(NICK, USER, CHANNEL) CRESPONSE(NICK + "!" + USER + "@" + HOST + " PART " + CHANNEL)
// # define ERR_NOTONCHANNEL(CHANNEL, USER) RESPONSE("442", USER, "Vous n'êtes pas dans le channel " + CHANNEL)

// // WHOIS //
// # define RPL_ENDOFWHOIS(USER) RESPONSE("318", USER, "Fin de whois")
// # define RPL_USERHOST(USER, NICK) RESPONSE("302", USER, USER + "=+" + NICK + "@" + HOST)

// // PING //
// # define RPL_PING(USER) RESPONSE("PONG", USER, "Pong")
// # define RPL_PONG(NAME) RESPONSE("PING", NAME, "Ping")

// // LUSERS //
// # define RPL_LUSERCLIENT(USER) RESPONSE("251", USER, "Utilisateurs connectés : " + itoa(Server::clients.size()))

// // GLOBAL //
// # define ERR_ALREADYREGISTRED(USER) RESPONSE("462", USER, "Vous etes déjà enregistré")
// # define ERR_NEEDMOREPARAMS(USER, CMD) RESPONSE("461", USER, "La commande " + CMD + " requiert plus de paramètres")
// # define ERR_UNKNOWNCOMMAND(CMD) RESPONSE("421", "*", "Commande inconnue : " + CMD)

