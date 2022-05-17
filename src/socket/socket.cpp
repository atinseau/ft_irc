#include "socket.hpp"

void	msgServer(std::string str)
{
	std::cerr << VIOLET << "---SERVER---  " << str << BLANC << std::endl;
}

Server::Server(void)
{
	int rc;
	int on = 1;
	pollfd tmp;

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
	tmp.fd = this->_sockServer;
	tmp.events = POLLIN;
	this->_pfds.push_back(tmp);
	msgServer("Server correctement initialiser...");
}

Server::~Server()
{
	for (int i = 0; this->_pfds[i].fd > 0; i++)
		close(this->_pfds[i].fd);
}

void Server::run(void)
{
	int rc, len, current_size;
	pollfd tmp;
	bool end_server, close_conn, compress_array;
	char buffer[200];
	do
	{
		msgServer("En attente de poll()...");
		if ((rc = poll(&(*this->_pfds.begin()), this->_pfds.size(), TIME)) <= 0)
			throw std::runtime_error(ROUGE"poll() failed/timeout"BLANC);
		current_size = this->_pfds.size();
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
					tmp.events = POLLIN;
					if ((tmp.fd = accept(this->_sockServer, NULL, NULL)) < 0)
					{
						if (errno != EWOULDBLOCK)
							throw std::runtime_error(ROUGE"accept() failed"BLANC);
						break;
					}
					std::cerr << VIOLET << "---SERVER---  " << "Nouvel connection ! fd : " << tmp.fd << BLANC << std::endl;
					this->_pfds.push_back(tmp);
				} while (tmp.fd != -1);
			}
			else
			{
				std::cerr << VIOLET << "---SERVER---  Descripteur" << this->_pfds[i].fd << " est lisible..." << BLANC << std::endl;
				close_conn = false;
				do
				{
					memset(&buffer, 0, 200);
					if ((rc = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0)) < 0)
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
					buffer[len] = '\0';
					//std::cerr << BLEU << "---SERVER---  Reception :" << len << " bits" << BLANC << std::endl;
					//std::cerr << BLEU << "---SERVER---  Reception :(" << buffer <<")" << BLANC << std::endl;
					std::cout << "(" << buffer << ")"<< std::endl;
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
		int tmp = this->_pfds.size();
		if (compress_array)
		{
			compress_array = false;
			for (int i = 0; i < tmp; i++)
			{
				if (this->_pfds[i].fd == -1)
				{
					for(int j = i; j < tmp - 1; j++)
					{
						this->_pfds[j].fd = this->_pfds[j+1].fd;
					}
					i--;
					tmp--;
				}
			}
		}
	} while (end_server == false); 
}