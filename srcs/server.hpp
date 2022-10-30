#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <vector>

class Server {
    public:
        Server(void);
        virtual ~Server(void);

        void    set_config(int port, char *password);
        void    run(void);
        bool    setup(void);
        bool    exec(std::string *all, unsigned int i);
        void    update_fds_all_users(int user_id);

        bool    exec_pass(std::string *value, unsigned int i);
        bool    exec_nick(std::string *value, unsigned int i, users *user);
        bool    exec_user(std::string *value, unsigned int i, users *user);
        bool    exec_msg(std::string *value, unsigned int i, users *user);
        void    exec_away(std::string *value, unsigned int i);
        bool    exec_join(std::string *value, unsigned int i, users *user);
        bool    exec_part(std::string *value, unsigned int i, users *user);
        bool    exec_oper(std::string *value, unsigned int i, users *user);
        bool    exec_kill(std::string *value, unsigned int i, users *user);
        bool    exec_shutdown(std::string *value);
        void    exec_quit(unsigned int i, users *user);
        void    exec_kick(std::string *value, unsigned int i);
    private:
        struct sockaddr_in  address;
        int                 server_fd;
        char                *password;
        int                 port;
        struct pollfd       fds[200];
        users               *all_users;
        std::vector<std::string> unavailable_nicknames;
        channel             *channels;
        int                 number_of_socket;
        int                 new_socket[200];
};

#endif
