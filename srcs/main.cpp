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

#include "store_commands.hpp"

#define PORT 6667
#define EXIT_FAILURE 1

int main(void)
{
    int server_fd;
    int new_socket[200];
    int socket_id = 0;
    struct sockaddr_in address;

    int rc;
    int nfds = 1;
    struct pollfd fds[200];

    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    std::map<std::string, void(*)(pollfd*, int&)> cmds = store_commands(); // Stockage des commandes dans une map

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
    address.sin_port = htons(PORT);

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

    unsigned int i = 0;
    while (true)
    {

        std::cout << "poll setup: " << nfds << std::endl;
        rc = poll(fds, nfds, -1);

        if (rc < 0)
            std::cout << "poll failed" << std::endl;

        if (rc == 1)
        {
            if (fds[0].revents == 1)
            {
                std::cout << "inserting new socket: " << nfds - 1 << std::endl;
                if ((new_socket[socket_id] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                std::cout << "connected" << std::endl;
                fds[nfds].fd = new_socket[socket_id];
                fds[nfds].events = POLLIN;
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
                        read(fds[i].fd, (char *)buffer, 1024);
                        if (strcmp(buffer, "exit\r\n") == 0 || strcmp(buffer, "exit\r\n\n") == 0)
                        {
                            close(fds[nfds - 1].fd);
                            nfds--;
                        }
                        if (cmds.find(buffer) != cmds.end())
                        {
                            cmds.find(buffer)->second(fds, nfds);
                        }
                        printf("%s", buffer);
                    }
                }
            }
        }
    }
    return (0);
}
