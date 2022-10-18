#include <iostream>
#include <map>
#include <functional>
#include <utility>
#include <sys/poll.h>
#include <unistd.h>
# define DBG(vari) std::cerr<<#vari<<" = "<<(vari)<<std::endl;

void exit(pollfd *fds, int& nfds)
{
    close(fds[nfds - 1].fd);
    nfds--;
    // DBG("ici")
    return ;
}

std::map<std::string, void(*)(pollfd*, int&)> store_commands()
{
    std::map<std::string, void(*)(pollfd*, int&)> commands;
    commands["exit\r\n"] = exit;
    commands["exit\r\n\n"] = exit;          // Hacky patch à modifier
    return (commands);
}