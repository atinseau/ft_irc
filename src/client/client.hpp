#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../irc.hpp"

#define NICKNAME_LENGTH 30

enum ClientState
{
	IS_AUTH,
	IS_NOT_AUTH,
	AFTER_AUTH
};

class Client
{
public:
	Client(pollfd pfd);
	~Client(void);

	Request read();

	void disconnect();
	void write(Response res);
	void join(std::string channel);
	bool leave(std::string channel);

	void remove_channel(const std::string& channel);

	void set_state(ClientState state);

	bool is_auth();
	std::string &operator[](const char *key);

	int get_fd() const;
	pollfd get_pfd();
	ClientState get_state() const;
	const std::string& get_key(const char *key) const;
	std::vector<std::string> get_info(bool print = false) const;
	std::string fullname() const;

	std::vector<Channel*> get_channels() const; 

private:
	pollfd _pfd;
	std::vector<std::string> _channels;
	std::map<const char *, std::string> _data;
	ClientState _state;
};

bool is_client_key_colliding(Client &client, const char *key, std::string &next);
Client *get_client_by_key(const char *key, const char *value);
Client* get_client_by_fd(int fd);
bool is_not_target(Client &initiator, Client &target);

#endif