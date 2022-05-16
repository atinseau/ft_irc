#include "socket.hpp"

void	msgServer(std::string str)
{
	std::cerr << VIOLET << "------------------------------------------------------" << std::endl;
	std::cerr << "SERVER : " << str << std::endl;
	std::cerr << "------------------------------------------------------" << BLANC << std::endl;
}

Server::Server(void)
{
	this->_sockServer = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->_sockServer < 0)
		throw std::runtime_error(ROUGE"socket() failed"BLANC);

	int opt = 0;
	if (setsockopt(this->_sockServer, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		close(this->_sockServer);
		throw std::runtime_error(ROUGE"setsockopt() failed"BLANC);
	}
	if (ioctl(this->_sockServer, FIONBIO, (char *)&opt) < 0)
	{
		close(this->_sockServer);
		throw std::runtime_error(ROUGE"ioctl() failed"BLANC);
	}
	memset(&_addrServer, 0, sizeof(_addrServer));
	//memset(&this->_pfds, 0 , sizeof(this->_pfds));
	_addrServer.sin6_family = AF_INET6;
	memcpy(&_addrServer.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	_addrServer.sin6_port        = htons(SERVER_PORT);
	if (bind(_sockServer, (struct sockaddr *)&_addrServer, sizeof(_addrServer)) < 0)
	{
		close(this->_sockServer);
		throw std::runtime_error(ROUGE"bind() failed"BLANC);
	}

	if (listen(_sockServer, 32) < 0)
	{
		close(this->_sockServer);
		throw std::runtime_error(ROUGE"listen() failed"BLANC);
	}

	pollfd pfd;

	pfd.fd = this->_sockServer;
	pfd.events = POLLIN;

	this->_pfds[0] = pfd;

}

Server::~Server()
{
	for (int i = 0; this->_pfds[i].fd > 0; i++)
	{
		close(this->_pfds[i].fd);
	} 
}

//void Server::addClient(int &lenTab)
//{
	
//}


void Server::run(void)
{
	int resPoll;
	int sizeVector;
	int lenTab = 1;

	while (1)
	{
		msgServer("Attente de poll()...");
		resPoll = poll(this->_pfds, lenTab, TIME);
		if (resPoll < 0)
		{
			throw std::runtime_error(ROUGE"Poll() failed"BLANC);
		}
		if (resPoll == 0)
		{
			msgServer("La fonction poll() est arrivée a la fin de sont timer...");
			break;
		}
		sizeVector = lenTab;
		for (int i = 0; i < sizeVector; i++)
		{
			if (this->_pfds[i].revents == 0)
				continue ;
			if (this->_pfds[i].revents != POLLIN)
			{
				throw std::runtime_error(ROUGE"ERREUR d unn client"BLANC);
			}
			if (this->_pfds[i].fd == this->_sockServer)
			{
				msgServer("Recherche de client en cours...");
				int fd;
				do 
				{
					if ((fd = accept(this->_sockServer, NULL, NULL)) <= 0)
					{
						close(fd);
						throw std::runtime_error(ROUGE"Accept() failed"BLANC);
					} 
					msgServer("connecter...");
					this->_pfds[lenTab].events = POLLIN;
					this->_pfds[lenTab].fd = fd;
					lenTab++;
				} while (fd != -1);
			}
			else
			{
				std::cerr << VIOLET << "------------------------------------------------------" << std::endl;
				std::cerr << "SERVER : " << "Fd : " << this->_pfds[i].fd << " est lisible..." << std::endl;
				std::cerr << "------------------------------------------------------" << BLANC << std::endl;
				int resRecv = 0;
				std::string tmp;
				while (1)
				{
					std::cout << "test ici" << std::endl;
					resRecv = recv(this->_pfds[i].fd, &tmp, sizeof(tmp), 0);
					if (resRecv < 0)
						throw std::runtime_error(ROUGE"Recv() failed"BLANC);
					if (resRecv == 0)
					{
						msgServer("Connection client fermée");
						break ;
					}
					std::cerr << VIOLET << "------------------------------------------------------" << std::endl;
					std::cerr << "SERVER : " << resRecv << " on etait ressus : \"" << tmp << " est lisible..." << std::endl;
					std::cerr << "------------------------------------------------------" << BLANC << std::endl;
					resRecv = send(this->_pfds[i].fd, &tmp, sizeof(tmp), 0);
					if (resRecv < 0)
						throw std::runtime_error(ROUGE"Send() failed"BLANC);
				}
				
			}

		}
	}
		
}