#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <string.h>
#include "channels.hpp"

void	send_connection_ok(int socket, std::string nickname);
void	send_nickname_already_used(int socket, std::string nickname);
void	send_err_password(int socket);
void    send_broadcast(unsigned int nb_fd, struct pollfd *fds, std::string message);
void	send_no_recipient(int socket, std::string command);
void	send_no_text(int socket);
void	send_no_such_nick(int socket, std::string nickname);
void	send_err_cannot_send_to_chan(int socket, std::string channel_name);
void	send_message_to_user(int socket, std::string nickname, std::string message_to_send, users *sender);
void	send_away_message_to_user(int socket, std::string nickname, std::string away_message);
void    send_rpl_namreply(channel *chan, std::string nickname, int socket, users *users_list);
void    send_rpl_topic(channel *chan, int socket);
void    send_no_such_channel(std::string channel_name, int socket);
void    send_not_on_channel(std::string channel_name, int socket);
void    send_need_more_params(std::string command, int socket);
void    send_away_message(int socket);
void    send_back_from_away_message(int socket);
void    send_already_registred(int socket);
void    send_user_joined_channel(int socket, std::string nickname, std::string username, std::string channel_name);
void    send_user_quit_answer(int socket);
void    send_no_privileges(int socket);

#endif
