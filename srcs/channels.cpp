#include "channels.hpp"

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
    while (tmp)
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

    if (!channels)
        return (NULL);
    while (tmp)
    {
        if (tmp->name == name)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
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

int find_channel_user(channel *chan, int user_id)
{
    for (int i = 0; i < chan->nb_users; i++)
    {
        if (chan->users_id[i] == user_id)
        {
            return (i);
        }
    }
    return (-1);
}

int remove_user_from_channels(channel *channels, int user_id, pollfd *fds, std::string nickname, std::string username, std::string part_msg)
{
    channel *tmp = channels;
    int i = 0;

    while (tmp)
    {
        i = find_channel_user(tmp, user_id);
        int tmp_id = tmp->users_id[i];
        if (i != -1)
        {
            for (; i < tmp->nb_users; ++i)
            {
                tmp->users_id[i] = tmp->users_id[i + 1];
            }
            for (int j = 0; j < tmp->nb_users; j++)
            {
                if (tmp->users_id[j] != 0 && tmp->users_id[j] > tmp_id)
                {
                    tmp->users_id[j]--; // Je décrémente tous les ids qui sont dans mon tableau d'int sinon ils ne s'actualisent pas avec update_fds_all_users()
                }
            }
            tmp->nb_users--;
            for (int it = 0; it < (tmp->nb_users); it++)
            {
                send_user_part_channel(fds[tmp->users_id[it] + 1].fd, nickname, username, tmp->name, part_msg);
            }
        }
        tmp = tmp->next;
    }
    return (0);
}
