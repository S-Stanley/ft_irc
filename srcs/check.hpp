#ifndef CHECK_HPP
#define CHECK_HPP

#include <cctype>
#include <cstring>
#include <cstdlib>

#include "response.hpp"
#include "users.hpp"

bool    only_digits(char *str);
int     check_args(int argc, char **argv);
void    check_password(users *all_users, pollfd *fds, std::string *value, char *password, unsigned int i);

#endif