#ifndef USER_HPP
# define USER_HPP

#include <iostream>

struct users {
    unsigned int    user_id;
    std::string     nickname;
    std::string     username;
    bool            authentificate;
    bool            connected;
    bool            is_operator;
    bool            away;
    std::string     away_message;
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
users   *find_user_by_nickname(std::string nickname, users *lst);
users   *set_user_away(int user_id, users *lst, std::string away_message);
users   *set_user_back_from_away(int user_id, users *lst);
bool    is_user_away(int user_id, users *lst);
users   *delete_user_from_list(int user_id, users *lst);

#endif
