#include "server.hpp"

Server::Server(void)
{
    this->all_users = NULL;
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

void    Server::run(void)
{
    int             rc;
    int             nfds = 1;
    int             new_socket[200];
    int             socket_id = 0;
    bool            stop_server = false;
    unsigned int    i = 0;
    int             addrlen = sizeof(address);


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
                while (i <= (unsigned int)nfds)
                {
                    i++;
                    if (fds[i].revents == 1)
                    {
                        std::string *all = get_commands(fds, i);
                        if (!all[0].find("PASS") && get_user(i - 1, all_users)->connected == false)
                        {
                            send_err_password(fds[i].fd);
                            continue;
                        }
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
                                    continue;
                                if (is_nickname_available(all_users, value[1]))
                                    all_users = set_user_nickname(all_users, i -1, value[1]);
                                else
                                    send_nickname_already_used(fds[i].fd, value[1]);
                            }
                            if (value[0] == "USER")
                            {
                                if (get_user(i - 1, all_users)->connected == false)
                                    continue;
                                all_users = set_user_username(all_users, i -1, value[1]);
                                if (strcmp(get_user(i -1, all_users)->nickname.c_str(), "") != 0)
                                {
                                    set_user_authentificate(i -1, all_users);
                                    send_connection_ok(fds[i].fd, get_user(i -1, all_users)->nickname);
                                }
                            }
                            if (value[0] == "SHUTDOWN")
                            {
                                stop_server = true;
                                break;
                            }
                        }
                        delete[] all;
                    }
                }
            }
        }
        if (stop_server)
            break;
    }
}

Server::~Server(void)
{
    free_all_users(all_users);
}