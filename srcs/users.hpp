#ifndef USER_HPP
# define USER_HPP

#include <iostream>

struct users {
    unsigned int    user_id;
    std::string     nickname;
    std::string     username;
    bool            authentificate;
    bool            connected;
    struct users    *next;
};

users   *new_user(int user_id, std::string nickname, std::string username);
users   *add_user(users *lst, users *new_user);
users   *set_user_nickname(users *lst, int user_id, std::string nickname);
users   *set_user_username(users *lst, int user_id, std::string username);
void    read_all_users(users *lst);
void    free_all_users(users *lst);
users   *get_user(int user_id, users *lst);
users   *set_user_authentificate(int user_id, users* lst);
bool    is_nickname_available(users *lst, std::string nickname);

#endif