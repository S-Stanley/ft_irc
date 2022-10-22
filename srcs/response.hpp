#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

void	send_connection_ok(int socket, std::string nickname);
void	send_nickname_already_used(int socket, std::string nickname);
void	send_err_password(int socket);

#endif