#include "server.hpp"

Server::Server(void)
{
    this->all_users = NULL;
    this->channels = NULL;
    this->number_of_socket = 1;
}

void    Server::set_config(int port, char *password)
{
    this->password = password;
    this->port = port;
}

bool    Server::setup(void)
{
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket failed\n";
        return (false);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        std::cerr << "setcokopt failed\n";
        return (false);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        std::cerr << "bind failed\n";
        return (false);
    }

    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "listen failed\n";
        return (false);
    }

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    return (true);
}

void    Server::update_fds_all_users(int user_id)
{
    for (int i = user_id; i < number_of_socket; ++i)
    {
        fds[i] = fds[i + 1];
        if (get_user(i, all_users))
		{
            get_user(i, all_users)->user_id--;
		}
    }
}

bool    Server::exec_pass(std::string *value, unsigned int i)
{
    if (!check_password(all_users, fds, value, password, i))
    {
        delete[] value;
        all_users = delete_user_from_list(i - 1, all_users);
        number_of_socket--;
        return (false);
    }
    return (true);
}

bool    Server::exec_nick(std::string *value, unsigned int i, users *user)
{
    if (user->connected == false)
    {
        delete[] value;
        return (true);
    }
    if (is_nickname_available(all_users, value[1]) && !is_banned_nickname(value[1], unavailable_nicknames))
        all_users = set_user_nickname(all_users, i -1, value[1]);
    else
        send_nickname_already_used(fds[i].fd, value[1]);
    return (false);
}

bool    Server::exec_user(std::string *value, unsigned int i, users *user)
{
    if (user->connected == false)
    {
        delete[] value;
        return (true);
    }
    all_users = set_user_username(all_users, i -1, value[1]);
    if (ft_strcmp(user->nickname.c_str(), "") != 0)
    {
        set_user_authentificate(i -1, all_users);
        send_connection_ok(fds[i].fd, user->nickname);
    }
    return (false);
}

bool    Server::exec_msg(std::string *value, unsigned int i, users *user)
{
    channel *chan_to_find = find_channel(value[1], channels);
    users   *user_to_find = find_user_by_nickname(value[1], all_users);

    if (!user_to_find && !chan_to_find)
    {
        send_no_such_nick(fds[i].fd, value[1]);
        delete[] value;
        return (true);
    }
    if (chan_to_find && find_channel_user(chan_to_find, i - 1) == -1)
    {
        send_not_on_channel(value[1], fds[i].fd);
        delete[] value;
        return (true);
    }
    if (value->length() == 2)
    {
        send_no_text(fds[i].fd);
        delete[] value;
        return (true);
    }
    if (user_to_find)
    {
        send_message_to_user(
            new_socket[user_to_find->user_id],
            value[1],
            append_to_string(value, 2),
            user,
            true
        );
    } else {
        channel *chan = find_channel(value[1], channels);
        for (int it = 0; it < (chan->nb_users); it++)
        {
            if (user->nickname != get_user(chan->users_id[it], all_users)->nickname)
            {
                std::cout << "sending message to: " << get_user(chan->users_id[it], all_users)->nickname << std::endl;
                send_message_to_user(
                    fds[chan->users_id[it] + 1].fd,
                    value[1],
                    append_to_string(value, 2),
                    user,
                    false
                );
            }
        }
    }
    if (value[0] == "PRIVMSG" && find_user_by_nickname(value[1], all_users) && is_user_away(find_user_by_nickname(value[1], all_users)->user_id, all_users))
    {
        send_away_message_to_user(
            fds[i].fd,
            value[1],
            get_user(find_user_by_nickname(value[1], all_users)->user_id, all_users)->away_message
        );
    }
    return (false);
}

void    Server::exec_away(std::string *value, unsigned int i)
{
    if (value[1].empty())
    {
        set_user_back_from_away(i -1, all_users);
        send_back_from_away_message(fds[i].fd);
    }
    else
    {
        set_user_away(i - 1, all_users, append_to_string(value, 1));
        send_away_message(fds[i].fd);
    }
}

bool    Server::exec_join(std::string *value, unsigned int i, users *user)
{
    if (value[1].empty())
    {
        send_need_more_params(value[0], fds[i].fd);
        delete[] value;
        return (true);
    }
    channel *chan;

    std::cout << "channel to find -> " << value[1] << std::endl;
    if (channel_exists(value[1], channels) == false)
    {
        chan = create_channel(value[1], "Default Topic");
        chan->users_id[chan->nb_users] = user->user_id;
        chan->nb_users++;
        channels = add_new_channel(channels, chan);
    }
    else
    {
        chan = find_channel(value[1], channels);
        if (find_channel_user(chan, user->user_id) != -1)
        {
            std::cout << "L'utilisateur est d??j?? membre du channel...\n"; // Que renvoyer ?'
            delete[] value;
            return (true);
        }
        chan->users_id[chan->nb_users] = user->user_id;
        chan->nb_users++;

        for (int it = 0; it < (chan->nb_users); it++)
        {
            send_user_joined_channel(
                fds[chan->users_id[it] + 1].fd,
                user->nickname,
                user->username,
                value[1]
            );
        }
    }
    send_rpl_topic(chan, fds[i].fd);
    send_rpl_namreply(chan, user->nickname, fds[i].fd, all_users);
    return (false);
}

bool    Server::exec_part(std::string *value, unsigned int i, users *user)
{
    if (value[1].empty())
    {
        send_need_more_params(value[0], fds[i].fd);
        delete[] value;
        return (true);
    }
    channel *chan;

    if (!channel_exists(value[1], channels))
    {
        send_no_such_channel(value[1], fds[i].fd);
        delete[] value;
        return (true);
    }
    chan = find_channel(value[1], channels);
    int u = find_channel_user(chan, user->user_id);
    if (u == -1)
    {
        send_not_on_channel(chan->name, fds[i].fd);
        delete[] value;
        return (true);
    }
    for (int it = 0; it < (chan->nb_users); it++)
    {
        if (value[2].empty() == false)
            send_user_part_channel(
                fds[chan->users_id[it] + 1].fd,
                user->nickname,
                user->username,
                value[1],
                append_to_string(value, 2)
            );
        else
            send_user_part_channel(
                fds[chan->users_id[it] + 1].fd,
                user->nickname,
                user->username,
                value[1],
                ""
            );
    }
    if (!value[2].empty() && find_user_by_nickname(value[2], all_users))
        send_not_on_channel(chan->name, new_socket[find_user_by_nickname(value[2], all_users)->user_id]);
    else
        send_not_on_channel(chan->name, fds[i].fd);
    for (int i = u; i < chan->nb_users; ++i)
        chan->users_id[i] = chan->users_id[i + 1];
    chan->nb_users--;
    delete[] value;
    return (true);
}

bool    Server::exec_oper(std::string *value, unsigned int i, users *user)
{
    if (value[1].empty() || value[2].empty())
    {
        send_need_more_params(value[0], fds[i].fd);
        delete[] value;
        return (true);
    }
    else if (user->is_operator)
    {
        // D??j?? operator
        delete[] value;
        return (true);
    }
    user->is_operator = true;
    send_youre_oper(fds[i].fd);
    return (false);
}

bool    Server::exec_kill(std::string *value, unsigned int i, users *user)
{
    users *user_to_kill;
    int update_at;
    if (value[1].empty() || value[2].empty())
    {
        send_need_more_params(value[0], fds[i].fd);
        delete[] value;
        return (true);
    }
    else if (user->is_operator == false)
    {
        send_no_privileges(fds[i].fd);
        delete[] value;
        return (true);
    }
    else if (!(user_to_kill = find_user_by_nickname(value[1], all_users)))
    {
        send_no_such_nick(fds[i].fd, value[1]);
        delete[] value;
        return (true);
    }
    update_at = user_to_kill->user_id;
    std::string nick = user_to_kill->nickname;
    std::string username = user_to_kill->username;
    close(fds[user_to_kill->user_id + 1].fd);
    unavailable_nicknames.push_back(user_to_kill->nickname);
    all_users = delete_user_from_list(user_to_kill->user_id, all_users);
    number_of_socket--;
    update_fds_all_users(update_at + 1);
    remove_user_from_channels(channels, update_at, fds, nick, username, "KILL");
    return (false);
}

bool    Server::exec_shutdown(std::string *value)
{
    delete_all_channels(channels);
    free_all_users(all_users);
    delete[] value;
    return (false);
}

void	Server::exec_quit(unsigned int i, users *user)
{
    std::string nick = user->nickname;
    std::string username = user->username;
    int         user_id = get_user(i - 1, all_users)->user_id;

    send_user_quit_answer(fds[i].fd);
    all_users = delete_user_from_list(i - 1, all_users);
    close(fds[i].fd);
    number_of_socket--;
    update_fds_all_users(i);
    remove_user_from_channels(channels, user_id, fds, nick, username, "QUIT");
}

void    Server::exec_kick(std::string *value, unsigned int i)
{
    std::cout << "received kick command\n";
    if (!get_user(i -1, all_users)->is_operator)
    {
        std::cout << "no privilege for kick\n";
        send_no_privileges(fds[i].fd);
        delete[] value;
    }
    else if (value->length() < 3 || value[1].empty() || value[2].empty())
    {
        std::cout << "missing params for kick \n";
        send_need_more_params(value[0], fds[i].fd);
        delete[] value;
    }
    else if (!channel_exists(value[1], channels))
    {
        std::cout << "channel does not exit for kick\n";
        send_no_such_channel(value[1], fds[i].fd);
        delete[] value;
    }
    else
    {
        exec_part(value, find_user_by_nickname(value[2], all_users)->user_id + 1, find_user_by_nickname(value[2], all_users));
    }
}

void    Server::exec_topic(std::string *value, unsigned int i, users *user)
{
    if (!user->is_operator)
    {
        send_no_privileges(fds[i].fd);
        return ;
    }
    else if (value[1].empty() || value[2].empty())
    {
        send_need_more_params(value[0], fds[i].fd);
        return ;
    }
    else if (!channel_exists(value[1], channels))
    {
        send_no_such_channel(value[1], fds[i].fd);
        return ;
    }

    channel *chan = find_channel(value[1], channels);
    if (find_channel_user(chan, user->user_id) == -1)
    {
        send_not_on_channel(chan->name, fds[i].fd);
        return ;
    }
    else
    {
        chan->topic = value[2];
        send_rpl_topic(chan, fds[i].fd);
    }
}

bool    Server::exec(std::string *all, unsigned int i)
{
    users *user = get_user(i -1, all_users);
    for (int x = 0; x < (int)all->length(); x++)
    {
        std::string *value = split_ft(all[x], " ");

        if (value[0] == "PASS")
            if (!exec_pass(value, i))
                return (true);
        if (value[0] == "NICK")
            if (exec_nick(value, i, user))
                continue;
        if (value[0] == "USER")
            if (exec_user(value, i, user))
                continue;
        if (value[0] == "PRIVMSG" || value[0] == "NOTICE")
            if (exec_msg(value, i, user))
                continue;
        if (value[0] == "AWAY")
            exec_away(value, i);
        if (value[0] == "JOIN")
            if (exec_join(value, i, user))
                return (true);
        if (value[0] == "PART")
            if (exec_part(value, i, user))
                return (true);
        if (value[0] == "OPER")                         // Pour l'instant tout le monde peut devenir operator
            if (exec_oper(value, i, user))
                return (true);
        if (value[0] == "KILL" || value[0] == "kill")
            if (exec_kill(value, i, user))
                return (true);
        if (value[0] == "KICK")
        {
            (exec_kick(value, i));
            continue;
        }
        if (value[0] == "QUIT")
        {
            exec_quit(i, user);
            if (!all_users)
                return (exec_shutdown(value));
        }
        if (value[0] == "TOPIC")
        {
            exec_topic(value, i, user);
            delete[] value;
            return true;
        }
        delete[] value;
    }
    return (true);
}

void    Server::run(void)
{
    int             rc;
    int             socket_id;
    bool            server_should_stop = true;
    unsigned int    i = 0;
    int             addrlen = sizeof(address);
    int             throw_err_password = true;
    users           *usr = NULL;

    while (true)
    {

        socket_id = number_of_socket - 1;
        read_all_users(all_users);
        rc = poll(fds, number_of_socket, -1);

        if (rc < 0)
            std::cout << "poll failed" << std::endl;

        if (rc == 1)
        {
            if (fds[0].revents == 1)
            {
                if ((new_socket[socket_id] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    std::cerr << "accept failed\n";
                    return ;
                }
                all_users = add_user(all_users, new_user(socket_id, "", ""));
                fds[number_of_socket].fd = new_socket[socket_id];
                fds[number_of_socket].events = POLLIN;
                send(fds[number_of_socket].fd, "Chatting in : <#Inserer le bon channel>\n", ft_strlen("Chatting in : <#Inserer le bon channel>\n"), 0);
                number_of_socket++;
                socket_id++;
            }
            else
            {
                i = 1;
                while (i < (unsigned int)number_of_socket)
                {
                    throw_err_password = true;
                    if (fds[i].revents == 1)
                    {
                        std::string *all = get_commands(fds, i);
                        for (int x = 0; x < (int)all->length(); x++)
                        {
                            usr = get_user(i - 1, all_users);
                            if (all[x].find("PASS") != std::string::npos || (usr && usr->connected == true))
                            {
                                throw_err_password = false;
                                break;
                            }
                        }
                        if (throw_err_password && get_user(i - 1, all_users)->connected == false)
                        {
                            send_err_password(fds[i].fd);
                            throw_err_password = true;
                        } else {
                            server_should_stop = this->exec(all, i);
                        }
                        delete[] all;
                    }
                    i++;
                }
            }
        }
        if (!server_should_stop)
            break;
    }
}

Server::~Server(void)
{
    free_all_users(all_users);
}
