#include "store_commands.hpp"
#include <cctype>
#include <cstring>
#include <cstdlib>

bool only_digits(char *str)
{
    for (unsigned int i = 0; i < strlen(str); i++)
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
        else if (atoi(argv[1]) < 1024)
        {
            std::cout << "Error: invalid port (1024 minimum)\n";
            return (-1);
        }
    }
    return (0);
}