#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../irc.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <map>
# define BLEU "\e[1;36m"
# define BLEU_2 "\e[1;34m"
# define ROUGE "\e[1;31m"
# define VERT "\e[1;32m"
# define JAUNE "\e[1;33m"
# define VIOLET "\e[1;35m"
# define BLANC "\e[0m"

#define SERVER_PORT 10000
#define TIME 3 * 60 * 1000

class Server
{
public:
	Server(std::string port, std::string password);
	~Server(void);
	void	playServer();					//programation du serveur
	void	run(void);						//protocole d ajoue des fd et de reception des message
	void	Reception(int i);				//receptionne les message
	void	closedAndPreventClient(int i);	//previent le client de la fermeture, ferme le fd du client et suprime l element du tableau 
	void	AddClient(void);				//ajoute les client 
private:
	int					_port;
	std::string			_password;
	int					_sockServer;
	struct sockaddr_in6	_addrServer;
	std::vector<pollfd>	_pfds;
	Server(void){};
};

void	msgServer(std::string str);

#endif