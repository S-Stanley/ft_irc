#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/socket.h>

#include "utils.hpp"

std::string *get_commands(struct pollfd *fds, int i);

#endif