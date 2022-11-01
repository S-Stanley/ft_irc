#include "utils.hpp"

std::string *split(std::string to_split, std::string split_at)
{
    size_t pos = 0;
    std::string *token = new std::string[100];
    unsigned int i = 0;

    while (i < to_split.size())
    {
        pos = to_split.find(split_at);
        if (pos == 0)
        {
            to_split.erase(0, 1);
            continue ;
        }
        if (pos > to_split.size())
        {
            token[i] = to_split.substr(0, to_split.size());
            to_split.erase(0, to_split.size());
            break ;
        }
        token[i] = to_split.substr(0, pos);
		if (i + 1 < to_split.size())
        	to_split.erase(0, pos + split_at.length());
        i++;
    }
    return (token);
}


std::string *split_ft(std::string to_split, std::string split_at)
{
    size_t pos = 0;
	size_t endpos;
    std::string *token = new std::string[100];
	unsigned int index = 0;
	std::string	val;

	while (to_split.find(split_at, pos) != std::string::npos)
	{
		if (pos != 0)
			pos = pos + 1;
		endpos = to_split.find(" ", pos + 1);
		val = to_split.substr(pos, endpos - pos);
		pos = endpos;
		token[index] = (val);
		index++;
	}
	return (token);
}


int	ft_iswhitespace(char c)
{
	if (c >= 9 && c <= 13)
	{
		return (1);
	}
	else if (c == 32)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int ft_atoi(const char *str)
{
	int			i;
	long long	res;
	long long	pos;

	i = 0;
	pos = 1;
	res = 0;
	if (!str)
		return (0);
	while (str[i] && ft_iswhitespace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			pos = pos * -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
		if (res > 2147483647 && pos == 1)
			return (-1);
		if (res > 2147483648 && pos == -1)
			return (0);
	}
	return (res * pos);
}

size_t	ft_strlen(const char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

size_t	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int i = 0;

	if (!s1 || !s2)
		return (-1);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

std::string append_to_string(std::string *arr, unsigned int key)
{
    std::string to_return;

    for (int i = key; i < (int)arr->length(); i++)
    {
        to_return.append(arr[i]);
        if (i + 1 != (int)arr->length())
            to_return.append(" ");
    }
    return (to_return);
}