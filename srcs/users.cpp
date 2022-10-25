#include "users.hpp"

users   *new_user(int user_id, std::string nickname, std::string username)
{
    users    *new_user = new users;

    new_user->user_id = (unsigned int)user_id;
    new_user->nickname = nickname;
    new_user->username = username;
    new_user->authentificate = false;
    new_user->connected = false;
    new_user->is_operator = false;
    new_user->next = NULL;
    return (new_user);
}

users   *add_user(users *lst, users *new_user)
{
    users *tmp = lst;

    if (!lst)
        return (new_user);
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_user;
    return (lst);
}

users   *set_user_nickname(users *lst, int user_id, std::string nickname)
{
    users *tmp = lst;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
            tmp->nickname = nickname;
        tmp = tmp->next;
    }
    return (lst);
}

users   *set_user_username(users *lst, int user_id, std::string username)
{
    users *tmp = lst;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
            tmp->username = username;
        tmp = tmp->next;
    }
    return (lst);
}

void    read_all_users(users *lst)
{
    users   *tmp = lst;

    if (!lst)
        return ;

    while (tmp)
    {
        std::cout << "user: " << tmp->user_id << " " << tmp->nickname << " " << tmp->username << std::endl;
        tmp = tmp->next;
    }
}

void    free_all_users(users *lst)
{
    users   *tmp;

    if (!lst)
        return ;
    while (lst)
    {
        tmp = lst->next;
        delete lst;
        lst = tmp;
    }
}

users    *get_user(int user_id, users *lst)
{
    users   *tmp = lst;

    while (tmp)
    {
        if ((unsigned int)user_id == tmp->user_id)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

users   *set_user_authentificate(int user_id, users* lst)
{
    users *tmp = lst;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
            tmp->authentificate = true;
        tmp = tmp->next;
    }
    return (lst);
}

bool    is_nickname_available(users *lst, std::string nickname)
{
    users   *tmp = lst;

    while (tmp)
    {
        if (tmp->nickname == nickname)
            return (false);
        tmp = tmp->next;
    }
    return (true);
}

users   *find_user_by_nickname(std::string nickname, users *lst)
{
    users   *tmp = lst;

    while (tmp)
    {
        if (tmp->nickname == nickname)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

users   *set_user_away(int user_id, users *lst, std::string away_message)
{
    users   *tmp = lst;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
        {
            tmp->away = true;
            tmp->away_message = away_message;
        }
        tmp = tmp->next;
    }
    return (lst);
}

users   *set_user_back_from_away(int user_id, users *lst)
{
    users   *tmp = lst;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
            tmp->away = true;
        tmp = tmp->next;
    }
    return (lst);
}

bool    is_user_away(int user_id, users *lst)
{
    users   *tmp = lst;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
        {
            if (tmp->away == true)
                return (true);
            else
                return (false);
        }
        tmp = tmp->next;
    }
    return (false);
}

users   *delete_user_from_list(int user_id, users *lst)
{
    users   *tmp = lst;
    users   *previous = NULL;

    while (tmp)
    {
        if (tmp->user_id == (unsigned int)user_id)
        {
            if (previous == NULL)
            {
                if (tmp->next)
                {
                    users *next = tmp->next;
                    delete tmp;
                    return (next);
                }
                delete tmp;
                return (NULL);
            }
            previous->next = tmp->next;
            delete tmp;
            return (lst);
        }
        previous = tmp;
        tmp = tmp->next;
    }

    return (lst);
}
