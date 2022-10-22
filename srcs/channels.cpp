#include "channels.hpp"
#define DBG(vari) std::cerr<<#vari<<" = "<<(vari)<<std::endl;

bool	channel_exists(std::string name, channel *channels)
{
    channel *tmp = channels;
    if (tmp == NULL)
    {
        return (false);
    }
    if (tmp->name == name)
    {
        return (true);
    }
    while (tmp->next)
    {
        DBG(tmp->name)
        if (tmp->name == name)
        {
            return (true);
        }
        tmp = tmp->next;
        DBG(tmp)
    }
    return (false);
}

channel *find_channel(std::string name, channel *channels)
{
    channel *tmp = channels;

    while (tmp->next)
    {
        if (tmp->name == name)
        {
            return (tmp);
        }
        tmp = tmp->next;
    }
    return (tmp);
}

channel *create_channel(std::string name, std::string topic)
{
    channel *chan = new channel;
    chan->name = name;
    chan->topic = topic;
    chan->users_list = NULL;
    chan->next = NULL;
    return (chan);
}

channel *add_new_channel(channel *channels, channel *new_channel)
{
    channel *tmp = channels;

    if (!channels)
        return (new_channel);
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    tmp->next = new_channel;
    return (channels);
}

users *add_user_channel(users *users_list, users* user)
{
    return (add_user(users_list, user));
}