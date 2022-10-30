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
int         ft_atoi(const char *str);
size_t	    ft_strlen(const char *str);
size_t	    ft_strcmp(const char *s1, const char *s2);

#endif