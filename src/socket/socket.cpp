#include "socket.hpp"

void	msgServer(std::string str)
{
	std::cerr << VIOLET << "------------------------------------------------------" << std::endl;
	std::cerr << "SERVER : " << str << std::endl;
	std::cerr << "------------------------------------------------------" << BLANC << std::endl;
}

Server::Server(void)
{
	int rc;
	int on = 1;

  this->_sockServer = socket(AF_INET6, SOCK_STREAM, 0);
  if (this->_sockServer < 0)
  {
    perror("socket() failed");
    exit(-1);
  }

  rc = setsockopt(this->_sockServer, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
  if (rc < 0)
  {
    perror("setsockopt() failed");
    close(this->_sockServer);
    exit(-1);
  }
  rc = ioctl(this->_sockServer, FIONBIO, (char *)&on);
  if (rc < 0)
  {
    perror("ioctl() failed");
    close(this->_sockServer);
    exit(-1);
  }
  memset(&this->_addrServer, 0, sizeof(this->_addrServer));
  this->_addrServer.sin6_family      = AF_INET6;
  memcpy(&this->_addrServer.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  this->_addrServer.sin6_port        = htons(SERVER_PORT);
  rc = bind(this->_sockServer,
            (struct sockaddr *)&this->_addrServer, sizeof(this->_addrServer));
  if (rc < 0)
  {
    perror("bind() failed");
    close(this->_sockServer);
    exit(-1);
  }
  rc = listen(this->_sockServer, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(this->_sockServer);
    exit(-1);
  }
  memset(this->_pfds, 0 , sizeof(this->_pfds));

  this->_pfds[0].fd = this->_sockServer;
  this->_pfds[0].events = POLLIN;
  
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

/*
void Server::run(void)
{
	int resPoll;
	int sizeVector;
	int lenTab = 1;
	int fd;

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
				do 
				{
					if ((fd = accept(this->_sockServer, NULL, NULL)) < 0)
					{
						close(fd);
						throw std::runtime_error(ROUGE"Accept() failed"BLANC);
					} 
					msgServer("connecter...");
					this->_pfds[lenTab].fd = fd;
					this->_pfds[lenTab].events = POLLIN;
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
		
}*/

void Server::run(void)
{
	int rc;
	int len;
	int new_sd;
	int nfds = 1;
	int current_size;
	int i;
	bool end_server;
	bool close_conn;
	bool compress_array;
	char buffer[200];
	do
  {
    printf("Waiting on poll()...\n");
    rc = poll(this->_pfds, nfds, TIME);
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }
    if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
    }
    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      if(this->_pfds[i].revents == 0)
        continue;
      if(this->_pfds[i].revents != POLLIN)
      {
        printf("  Error! revents = %d\n", this->_pfds[i].revents);
        end_server = true;
        break;
      }
      if (this->_pfds[i].fd == this->_sockServer)
      {
        printf("  Listening socket is readable\n");
        do
        {
         
          new_sd = accept(this->_sockServer, NULL, NULL);
          if (new_sd < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  accept() failed");
              end_server = true;
            }
            break;
          }
          printf("  New incoming connection - %d\n", new_sd);
          this->_pfds[nfds].fd = new_sd;
          this->_pfds[nfds].events = POLLIN;
          nfds++;
        } while (new_sd != -1);
      }
      else
      {
        printf("  Descriptor %d is readable\n", this->_pfds[i].fd);
        close_conn = false;
        do
        {
          rc = recv(this->_pfds[i].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = true;
            }
            break;
          }
          if (rc == 0)
          {
            printf("  Connection closed\n");
            close_conn = true;
            break;
          }
          len = rc;
          printf("  %d bytes received\n", len);
          rc = send(this->_pfds[i].fd, buffer, len, 0);
          if (rc < 0)
          {
            perror("  send() failed");
            close_conn = true;
            break;
          }

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
      for (i = 0; i < nfds; i++)
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