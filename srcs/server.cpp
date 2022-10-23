#include "server.hpp"

Server::Server(void)
{
    this->all_users = NULL;
    this->channels = NULL;
    return ;
}

void    Server::set_config(int port, char *password)
{
    this->password = password;
    this->port = port;
}

void    Server::setup(void)
{
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setcokopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
}

bool    Server::exec(std::string *all, unsigned int i)
{
    for (int x = 0; x < (int)all->length(); x++)
    {
        std::string *value = split(all[x], " ");
        if (value[0] == "PASS")
        {
            check_password(all_users, fds, value, password, i);
        }
        if (value[0] == "NICK")
        {
            if (get_user(i - 1, all_users)->connected == false)
            {
                delete[] value;
                continue;
            }
            if (is_nickname_available(all_users, value[1]))
                all_users = set_user_nickname(all_users, i -1, value[1]);
            else
                send_nickname_already_used(fds[i].fd, value[1]);
        }
        if (value[0] == "USER")
        {
            if (get_user(i - 1, all_users)->connected == false)
            {
                delete[] value;
                continue;
            }
            all_users = set_user_username(all_users, i -1, value[1]);
            if (strcmp(get_user(i -1, all_users)->nickname.c_str(), "") != 0)
            {
                set_user_authentificate(i -1, all_users);
                send_connection_ok(fds[i].fd, get_user(i -1, all_users)->nickname);
            }
        }
        if (value[0] == "PRIVMSG" || value[0] == "NOTICE")
        {
            if (!find_user_by_nickname(value[1], all_users))
            {
                send_no_such_nick(fds[i].fd, value[1]);
                return (true);
            }
            if (value->length() == 2)
            {
                send_no_text(fds[i].fd);
                return (true);
            }
            send_message_to_user(
                fds[find_user_by_nickname(value[1], all_users)->user_id + 1].fd,
                value[1],
                value[2]
            );
        }
        if (value[0] == "JOIN")
        {
            if (value[1].empty())
            {
                send_need_more_params(value[0], fds[i].fd);
                return (true);
            }
            channel *chan;

            if (channel_exists(value[1], channels) == false)
            {
                chan = create_channel(value[1], "Default Topic");
                chan->users_id[chan->nb_users] = get_user(i -1, all_users)->user_id;
                chan->nb_users++;
                channels = add_new_channel(channels, chan);
                send_rpl_topic(chan, fds[i].fd);
                send_rpl_namreply(chan, get_user(i -1, all_users)->nickname, fds[i].fd, all_users);
            }
            else
            {
                chan = find_channel(value[1], channels);
                if (find_channel_user(chan, get_user(i -1, all_users)) != -1)
                {
                    std::cout << "L'utilisateur est déjà membre du channel...\n"; // Que renvoyer ?
                    return (true);
                }
                chan->users_id[chan->nb_users] = get_user(i -1, all_users)->user_id;
                chan->nb_users++;
                send_rpl_topic(chan, fds[i].fd);
                send_rpl_namreply(chan, get_user(i -1, all_users)->nickname, fds[i].fd, all_users);
            }
        }
        if (value[0] == "PART")
        {
            if (value[1].empty())
            {
                send_need_more_params(value[0], fds[i].fd);
                return (true);
            }
            channel *chan;

            if (channel_exists(value[1], channels))
            {
                chan = find_channel(value[1], channels);
                int u = find_channel_user(chan, get_user(i -1, all_users));
                if (u == -1)
                {
                    send_not_on_channel(chan->name, fds[i].fd);
                    return (true);
                }
                else
                {
                    for (int i = u; i < chan->nb_users; ++i)
                        chan->users_id[i] = chan->users_id[i + 1];
                    chan->nb_users--;
                }
            }
            else
            {
                send_no_such_channel(value[1], fds[i].fd);
                return (true);
            }
        }
        if (value[0] == "SHUTDOWN")
        {
            delete[] value;
            return (false);
        }
        delete[] value;
    }
    return (true);
}

void    Server::run(void)
{
    int             rc;
    int             nfds = 1;
    int             new_socket[200];
    int             socket_id = 0;
    bool            server_should_stop = true;
    unsigned int    i = 0;
    int             addrlen = sizeof(address);
    int             throw_err_password = true;

    while (true)
    {

        read_all_users(all_users);
        rc = poll(fds, nfds, -1);

        if (rc < 0)
            std::cout << "poll failed" << std::endl;

        if (rc == 1)
        {
            if (fds[0].revents == 1)
            {
                if ((new_socket[socket_id] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                all_users = add_user(all_users, new_user(socket_id, "", ""));
                fds[nfds].fd = new_socket[socket_id];
                fds[nfds].events = POLLIN;
                send(fds[nfds].fd, "Chatting in : <#Inserer le bon channel>\n", strlen("Chatting in : <#Inserer le bon channel>\n"), 0);
                nfds++;
                socket_id++;
            }
            else
            {
                i = 0;
                while (i < (unsigned int)nfds)
                {
                    i++;
                    if (fds[i].revents == 1)
                    {
                        std::string *all = get_commands(fds, i);
                        for (int x = 0; x < (int)all->length(); x++)
                        {
                            if (all[x].find("PASS") != std::string::npos || get_user(i - 1, all_users)->connected == true)
                            {
                                throw_err_password = false;
                                break;
                            }
                        }
                        if (throw_err_password)
                        {
                            send_err_password(fds[i].fd);
                            throw_err_password = true;
                        } else {
                            server_should_stop = this->exec(all, i);
                        }
                        delete[] all;
                    }
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