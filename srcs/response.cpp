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

void    send_broadcast(unsigned int nb_fd, struct pollfd *fds, std::string message)
{
    unsigned int i = 1;

    while (i < nb_fd)
    {
        std::cout << "sending to: " << i << std::endl;
        send(fds[i].fd, message.c_str(), strlen(message.c_str()), 0);
        i++;
    }
}

void	send_no_recipient(int socket, std::string command)
{
	std::string message = ":127.0.0.1 411 ERR_NORECIPIENT :No recipient given " + command +" \r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_no_text(int socket)
{
	std::string message = ":127.0.0.1 412 ERR_NOTEXTTOSEND :No text to send\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_no_such_nick(int socket, std::string nickname)
{
	std::string message = ":127.0.0.1 401 ERR_NOSUCHNICK " + nickname + " :No such nick/channel\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_err_cannot_send_to_chan(int socket, std::string channel_name)
{
	std::string message = ":127.0.0.1 404 ERR_CANNOTSENDTOCHAN " + channel_name + " :Cannot send to channel\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_message_to_user(int socket, std::string nickname, std::string message_to_send)
{
	std::string message = ":127.0.0.1 stan!stan@127.0.0.1 PRIVMSG " + nickname + " :" + message_to_send + "\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_away_message_to_user(int socket, std::string nickname, std::string away_message)
{
	std::string message = ":stan!stan@127.0.0.1 301 RPL_AWAY " + nickname + " :" + away_message +"\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void    send_rpl_namreply(channel *chan, std::string nickname, int socket, users *users_list)
{
    std::cout << nickname << std::endl;
    std::string message = "= " + chan->name + ": " + "@" + nickname + "\n";
    send(socket, message.c_str(), message.length(), 0);
    for (int i = 0; i < chan->nb_users; i++)
    {
        message = "@ " + get_user(chan->users_id[i], users_list)->nickname + "\n";
        send(socket, message.c_str(), message.length(), 0);
    }
}

void    send_rpl_topic(channel *chan, int socket)
{
    std::string message;
    if (chan->topic == "Default Topic")
        message = chan->name + " :" + "No topic is set\n";
    else
        message = chan->name + " :" + chan->topic + "\n";
    send(socket, message.c_str(), message.length(), 0);
}