#include "response.hpp"

void	send_connection_ok(int socket, std::string nickname)
{
	std::string message = ":127.0.0.1 001 RPL_WELCOME " + nickname + " :Welcome to the Internet Relay Network\r\n";
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

void	send_message_to_user(int socket, std::string nickname, std::string message_to_send, users *sender, bool priv)
{
    std::string message;
    if (priv)
	    message = ":127.0.0.1 " + sender->nickname + "!" + sender->username + "@127.0.0.1 PRIVMSG " + nickname + " " + message_to_send + "\r\n";
    else
	    message = ":" + sender->nickname + "!" + sender->username + "@127.0.0.1 PRIVMSG " + nickname + " " + message_to_send + "\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void	send_away_message_to_user(int socket, std::string nickname, std::string away_message)
{
	std::string message = ":127.0.0.1 301 RPL_AWAY :" + nickname + " :" + away_message + "\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void    send_rpl_namreply(channel *chan, std::string nickname, int socket, users *users_list)
{
    std::string all_names;

    for (int i = 0; i < chan->nb_users; i++)
    {
        if (i == 0)
            all_names = all_names.insert(all_names.length(), get_user(chan->users_id[i], users_list)->nickname);
        else
            all_names = all_names.insert(all_names.length(), " " + get_user(chan->users_id[i], users_list)->nickname);
    }

    std::string message = ":127.0.0.1 353 RPL_NAMREPLY " + nickname + " = " + chan->name + " :" + all_names + "\r\n";
    send(socket, message.c_str(), message.length(), 0);

    std::string end_of_name_message = ":127.0.0.1 366 RPL_ENDOFNAMES " + nickname + " " + chan->name + " :End of NAMES list\r\n";
    send(socket, end_of_name_message.c_str(), end_of_name_message.length(), 0);
}

void    send_rpl_topic(channel *chan, int socket)
{
    std::string message;
    if (chan->topic == "Default Topic")
        message = ":127.0.0.1 331 RPL_NOTOPIC " + chan->name + " :No topic is set\r\n";
    else
        message = ":127.0.0.1 332 RPL_TOPIC " + chan->name + " " + chan->topic + "\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_user_joined_channel(int socket, std::string nickname, std::string username, std::string channel_name)
{
    std::string message = ":" + nickname + "!" + username + "@127.0.0.1 JOIN " + channel_name + "\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_user_part_channel(int socket, std::string nickname, std::string username, std::string channel_name, std::string part_msg)
{
    std::string message = ":" + nickname + "!" + username + "@127.0.0.1 PART " + channel_name + " Reason: " + part_msg + "\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_no_such_channel(std::string channel_name, int socket)
{
    std::string message = ":127.0.0.1 403 ERR_NOSUCHCHANNEL " + channel_name + " :No such channel\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_not_on_channel(std::string channel_name, int socket)
{
    std::string message = ":127.0.0.1 442 ERR_NOTONCHANNEL " + channel_name + " :You're not on that channel\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_need_more_params(std::string command, int socket)
{
    std::string message = ":127.0.0.1 461 ERR_NEEDMOREPARAMS " + command + " :Not enough parameters\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_away_message(int socket)
{
	std::string message = ":127.0.0.1 306 RPL_NOWAWAY :You have been marked as being away\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void    send_back_from_away_message(int socket)
{
	std::string message = ":127.0.0.1 305 RPL_UNAWAY :You are no longer marked as being away\r\n";
	send(socket, message.c_str(), message.length(), 0);
}

void    send_already_registred(int socket)
{
    std::string message = ":127.0.0.1 462 ERR_ALREADYREGISTRED :Unauthorized command (already registered)\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_user_quit_answer(int socket)
{
    std::string message = ":127.0.0.1 ERROR :user quitted the server\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_no_privileges(int socket)
{
    std::string message = ":127.0.0.1 481 ERR_NOPRIVILEGES :Permission Denied- You're not an IRC operator\r\n";
    send(socket, message.c_str(), message.length(), 0);
}

void    send_youre_oper(int socket)
{
    std::string message = ":127.0.0.1 381 RPL_YOUREOPER :You are now an IRC operator\r\n";
    send(socket, message.c_str(), message.length(), 0);
}


