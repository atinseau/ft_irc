#include "command.hpp"

using namespace std;

Command::command_t Command::parse(const std::string &str)
{
	vector<string> args = split(str.c_str(), ' ');
	if (args.size() == 0)
		throw std::runtime_error("La commande est vide, cas de figure non géré");
	std::string cmd = args[0];
	args.erase(args.begin());
	return (std::make_pair(cmd, args));

}