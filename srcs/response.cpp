#include "response.hpp"

void	send_connection_ok(int socket, std::string nickname)
{
	std::string message = ":127.0.0.1 001 RPL_WELCOME" + nickname + " :Welcome to the Internet Relay Network\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_nickname_already_used(int socket, std::string nickname)
{
	std::cout << "nickname already use" << std::endl;
	std::string message = ":127.0.0.1 433 * " + nickname + " :Nickname is already in use\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_err_password(int socket)
{
	std::string message = ":127.0.0.1 464 ERR_PASSWDMISMATCH :Password incorrect\r\n";
	send(socket, message.c_str(), message.length(), 0);
}