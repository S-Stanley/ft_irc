#include "check.hpp"

bool only_digits(char *str)
{
    for (unsigned int i = 0; i < ft_strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            return (false);
        }
    }
    return (true);
}

int check_args(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Error: invalid number of arguments\n";
        return (-1);
    }
    else
    {
        if (!only_digits(argv[1]))
        {
            std::cout << "Error: invalid port (only digits)\n";
            return (-1);
        }
        else if (ft_atoi(argv[1]) < 1024)
        {
            std::cout << "Error: invalid port (1024 minimum)\n";
            return (-1);
        }
    }
    return (0);
}

bool    check_password(users *all_users, pollfd *fds, std::string *value, char *password, unsigned int i)
{
    users   *usr = get_user(i - 1, all_users);

    if (usr && usr->connected)
    {
        send_already_registred(fds[i].fd);
        return (false);
    }
    else if (value[1].empty())
    {
        send_need_more_params(value[0], fds[i].fd);
        return (false);
    }
    else if (!value[2].empty())
    {
        send(fds[i].fd, "Error: invalid password (no spaces)\n", ft_strlen("Error: invalid password (no spaces)\n"), 0);
        return (false);
    }
    else if (value[1] != password)
    {
        send_err_password(fds[i].fd);
        return (false);
    }
    else
    {
        get_user(i - 1, all_users)->connected = true;
        send(fds[i].fd, "Password OK\n", ft_strlen("Password OK\n"), 0);
        return (true);
    }
}

bool    is_banned_nickname(std::string nickname, std::vector<std::string> unavailable_nicks)
{
    for (unsigned int i = 0; i < unavailable_nicks.size(); i++)
    {
        if (unavailable_nicks[i] == nickname)
            return (true);
    }
    return (false);
}