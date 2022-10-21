#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>

struct server
{
    int fd;
    char *pw;
    int port;
};

std::string *split(std::string to_split, std::string split_at);

#endif