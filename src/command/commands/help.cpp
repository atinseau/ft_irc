#include "../command.hpp"

void Command::help(Payload& p)
{
	Client& client = p.client;

	const std::string& nickname = client.get_key("NICKNAME");

	const char* msg[] = {
		"------------------ HELP ------------------",
		"Voici la liste des commandes disponibles :",
		"------------------------------------------",
		"NICK <nickname>                     | Modifie votre pseudo",
		"JOIN #<channel>                     | Rejoint un channel",
		"PART #<channel>                     | Quitte un channel",
		"MODE #<channel> <mode> <?username>  | Modifie le mode d'un channel",
		"TOPIC #<channel> <?topic>           | Modifie le topic d'un channel",
		"QUIT                                | Quitte le serveur",
		"NAMES #<?channel>                   | Affiche la liste des utilisateurs d'un channel ou des channels",
		"LIST [#<?channel>]                  | Affiche la liste des channels ainsi que leurs topics",
		"INVITE <username> #<channel>        | Invite un utilisateur dans un channel",
		"KICK <channel> <username>           | Expulse un utilisateur d'un channel si vous êtes operateur",
		"PRIVMSG <username> <message>        | Envoie un message à un utilisateur",
		"HELP                                | Affiche cette liste",
	};

	for (size_t i = 0; i < sizeof(msg) / sizeof(char*); i++)
		client.write(RPL_INFO(nickname, msg[i]));
	
}