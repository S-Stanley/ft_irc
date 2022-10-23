#include "commands.hpp"

std::string *get_commands(struct pollfd *fds, int i)
{
    std::string *to_return = new std::string[100];
    int         valread;
    int         x = 0;
    char buffer[1024] = {0};

    valread = read(fds[i].fd, (char *)buffer, 1024);
    buffer[valread] = 0;
    std::string *tmp = split(buffer, "\n");
    for (int it = 0; it < (int)tmp->length(); it++)
    {
        std::string *all_test = split(tmp[it], "\r");
        to_return[x] = (all_test[0].c_str());
        delete[] all_test;
        x++;
    }
    delete[] tmp;
    return (to_return);
}
