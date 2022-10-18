#include <iostream>
#include <map>
#include <functional>
#include <utility>
#include <sys/poll.h>
#include <unistd.h>
# define DBG(vari) std::cerr<<#vari<<" = "<<(vari)<<std::endl;

void exit(pollfd *fds, int& nfds, unsigned int i)
{
    close(fds[i].fd);
    nfds--;
    return ;
}

// void join(pollfd *fds, int& nfds)
// {

// }

// void createChannel(pollfd *fds, int& nfds)
// {

// }

std::map<std::string, void(*)(pollfd*, int&, unsigned int)> store_commands()
{
    std::map<std::string, void(*)(pollfd*, int&, unsigned int)> commands;
    commands["exit\r\n"] = exit;
    commands["exit\r\n\n"] = exit;          // Hacky patch à modifier
    // commands["join\r\n"] = join;
    // commands["create.channel\r\n"] = createChannel;
    return (commands);
}