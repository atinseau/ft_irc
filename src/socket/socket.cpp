#include "socket.hpp"

void	msgServer(std::string str)
{
	std::cerr << VIOLET << "---SERVER---  " << str << BLANC << std::endl;
}

Server::Server(void)
{
	int rc;
	int on = 1;

	if ((this->_sockServer = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error(ROUGE"socket() failed"BLANC);
	if ((rc = setsockopt(this->_sockServer, SOL_SOCKET,  SO_REUSEADDR,(char *)&on, sizeof(on))) < 0)
		throw std::runtime_error(ROUGE"setsockopt() failed"BLANC);
	if ((rc = ioctl(this->_sockServer, FIONBIO, (char *)&on)) < 0)
		throw std::runtime_error(ROUGE"ioctl() failed"BLANC);
	memset(&this->_addrServer, 0, sizeof(this->_addrServer));
	this->_addrServer.sin6_family      = AF_INET6;
	memcpy(&this->_addrServer.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	this->_addrServer.sin6_port        = htons(SERVER_PORT);
	if ((rc = bind(this->_sockServer,(struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer))) < 0)
			throw std::runtime_error(ROUGE"bind() failed"BLANC);
	if ((rc = listen(this->_sockServer, 32)) < 0)
				throw std::runtime_error(ROUGE"listen() failed"BLANC);
	memset(this->_pfds, 0 , sizeof(this->_pfds));
	this->_pfds[0].fd = this->_sockServer;
	this->_pfds[0].events = POLLIN;
	msgServer("Server correctement initialiser...");
}

Server::~Server()
{
	for (int i = 0; this->_pfds[i].fd > 0; i++)
	{
		close(this->_pfds[i].fd);
	} 
}

void Server::run(void)
{
	int rc, len, new_sd, current_size;
	int nfds = 1;
	bool end_server, close_conn, compress_array;
	char buffer[200];
	do
	{
		msgServer("En attente de poll()...");
		rc = poll(this->_pfds, nfds, TIME);
		if (rc < 0)
			throw std::runtime_error(ROUGE"poll() failed"BLANC);
		if (rc == 0)
			throw std::runtime_error(ROUGE"poll() timeout"BLANC);
		current_size = nfds;
		for (int i = 0; i < current_size; i++)
		{
			if(this->_pfds[i].revents == 0)
				continue;
			if(this->_pfds[i].revents != POLLIN)
			{
				std::cout << ROUGE << "Error! revents = " << this->_pfds[i].revents << BLANC << std::endl;
				end_server = true;
				break;
			}
			if (this->_pfds[i].fd == this->_sockServer)
			{
				msgServer("Listening socket is readable");
				do
				{
					new_sd = accept(this->_sockServer, NULL, NULL);
					if (new_sd < 0)
					{
						if (errno != EWOULDBLOCK)
							throw std::runtime_error(ROUGE"accept() failed"BLANC);
						break;
					}
					std::cerr << VIOLET << "---SERVER---  " << "Nouvel connection ! fd : " << new_sd << BLANC << std::endl;
					this->_pfds[nfds].fd = new_sd;
					this->_pfds[nfds].events = POLLIN;
					nfds++;
				} while (new_sd != -1);
			}
			else
			{
				std::cerr << VIOLET << "---SERVER---  Descripteur" << this->_pfds[i].fd << " est lisible..." << BLANC << std::endl;
				close_conn = false;
				do
				{
					rc = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
							throw std::runtime_error(ROUGE"recv() failed"BLANC);
						break;
					}
					if (rc == 0)
					{
						msgServer("Connection closed\n");
						close_conn = true;
						break;
					}
					len = rc;
					std::cerr << BLEU << "---SERVER---  Reception : " << len << " bits" << BLANC << std::endl;
					std::cerr << BLEU << "---SERVER---  Reception : \" " << buffer << " \"" << BLANC << std::endl;
					rc = send(this->_pfds[i].fd, buffer, len, 0);
					if (rc < 0)
						throw std::runtime_error(ROUGE"send() failed"BLANC);
				} while(true);
				if (close_conn)
				{
					close(this->_pfds[i].fd);
					this->_pfds[i].fd = -1;
					compress_array = true;
				}
			}
		} 
		if (compress_array)
		{
			compress_array = false;
			for (int i = 0; i < nfds; i++)
			{
				if (this->_pfds[i].fd == -1)
				{
					for(int j = i; j < nfds-1; j++)
					{
						this->_pfds[j].fd = this->_pfds[j+1].fd;
					}
					i--;
					nfds--;
				}
			}
		}
	} while (end_server == false); /* End of serving running.    */
}