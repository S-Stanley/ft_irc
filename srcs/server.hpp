#ifndef SERVER_HPP
#define SERVER_HPP

#define EXIT_FAILURE 1

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>

#include "users.hpp"
#include "response.hpp"
#include "utils.hpp"
#include "commands.hpp"
#include "check.hpp"
#include "channels.hpp"
#define DBG(vari) std::cerr<<#vari<<" = "<<(vari)<<std::endl;

class Server {
    public:
        Server(void);
        virtual ~Server(void);

        void    set_config(int port, char *password);
        void    run(void);
        void    setup(void);
        bool    exec(std::string *all, unsigned int i);
    private:
        struct sockaddr_in  address;
        int                 server_fd;
        char                *password;
        int                 port;
        struct pollfd       fds[200];
        users               *all_users;
        channel             *channels;
};

#endif