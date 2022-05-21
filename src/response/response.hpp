#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <sstream>

# define HOST "localhost"

# define RESPONSE(CODE, USER, MSG) std::ostringstream() << ":" << HOST << " " << CODE << " " << USER << " :" << MSG << "\r\n"

// PASS
# define ERR_NEEDMOREPARAMS(USER) RESPONSE(461, USER, "Argument(s) manquant(s)")
# define ERR_ALREADYREGISTRED(USER) RESPONSE(462, USER, "Vous êtes déjà enregistré")

// NICK
# define ERR_NONICKNAMEGIVEN RESPONSE(431, "*", "Aucun pseudo n'a été donné")
# define ERR_NICKNAMEINUSE(USER) RESPONSE(433, USER, "Pseudo déjà utilisé")
# define ERR_ERRONEUSNICKNAME(USER) RESPONSE(432, USER, "Pseudo invalide (contient des caractères invalides ou trop long)")

class Response
{
	public:
		Response(std::ostringstream os);
		std::string str() const;
	private:
		std::string _str;
};

#endif