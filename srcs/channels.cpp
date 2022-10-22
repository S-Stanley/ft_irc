#include "channels.hpp"
#define DBG(vari) std::cerr<<#vari<<" = "<<(vari)<<std::endl;

bool	channel_exists(std::string name, channel *channels)
{
    channel *tmp = channels;
    if (tmp == NULL)
    {
        return (false);
    }
    else if (tmp->name == name)
    {
        return (true);
    }
    while (tmp->next)
    {
        if (tmp->name == name)
        {
            return (true);
        }
        tmp = tmp->next;
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
    chan->next = NULL;
    chan->nb_users = 0;
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

void    display_channel_users(channel *chan)
{
    channel *tmp = chan;
    int i = 0;
    while (i < tmp->nb_users)
    {
        std::cout << "User: " <<  i << " - user_id: " << tmp->users_id[i] << std::endl;
        i++;
    }
}
