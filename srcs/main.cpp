#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#include <list>
#include "check_args.hpp"

#include "store_commands.hpp"
#include "users.hpp"
#include "utils.hpp"
#include "response.hpp"

// #define PORT 6667
#define EXIT_FAILURE 1

struct server
{
    int fd;
    char *pw;
    int port;
};

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

int main(int argc, char **argv)
{
    if (check_args(argc, argv) == -1)
    {
        return (0);
    }

    server serv;
    serv.port = atoi(argv[1]);
    serv.pw = argv[2];

    int new_socket[200];
    int socket_id = 0;
    struct sockaddr_in address;
    users    *all_users = NULL;
    int valread;

    int rc;
    int nfds = 1;
    struct pollfd fds[200];

    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    std::map<std::string, void(*)(pollfd*, int&, unsigned int)> cmds = store_commands(); // Stockage des commandes dans une map

    if ((serv.fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(serv.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setcokopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(serv.port);

    if (bind(serv.fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if (listen(serv.fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    fds[0].fd = serv.fd;
    fds[0].events = POLLIN;

    unsigned int i = 0;
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
                if ((new_socket[socket_id] = accept(serv.fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
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
                        valread = read(fds[i].fd, (char *)buffer, 1024);
                        buffer[valread] = 0;
                        std::string *all = split(buffer, "\n");
                        for (int x = 0; x < 5; x++)
                        {
                            std::string *value = split(all[x], " ");
                            if (value[0] == "NICK")
                            {
                                if (is_nickname_available(all_users, value[1]))
                                    all_users = set_user_nickname(all_users, i -1, value[1]);
                                else
                                    send_nickname_already_used(fds[i].fd, value[1]);
                            }
                            if (value[0] == "USER")
                            {
                                all_users = set_user_username(all_users, i -1, value[1]);
                                if (strcmp(get_user(i -1, all_users)->nickname.c_str(), "") != 0)
                                {
                                    set_user_authentificate(i -1, all_users);
                                    send_connection_ok(fds[i].fd, get_user(i -1, all_users)->nickname);
                                }
                            }
                        }
                        delete[] all;
                        // DBG(buffer)
                        if (cmds.find(buffer) != cmds.end())    // On cherche dans la map si la commande existe
                        {
                            cmds.find(buffer)->second(fds, nfds, i);     // On exécute la fonction associée
                        }
                        else               // Ce n'est pas une commande connue, on envoie le message à tous les utilisateurs présents dans le channel
                        {
                            for (unsigned int j = 1; j <= (unsigned int)nfds; j++)
                            {
                                if (j != i)
                                {
                                    send(fds[j].fd, buffer, strlen(buffer), 0);
                                }
                            }
                        }
                        std::cout << buffer;
                    }
                }
                if (strcmp(buffer, "SHUTDOWN\r\n") == 0)
                {
                    break;
                }
            }
        }
    }
    free_all_users(all_users);
    return (0);
}
