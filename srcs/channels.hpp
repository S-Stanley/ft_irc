#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include "users.hpp"
# include <iostream>
# include <list>

struct channel
{
     std::string name;
     std::string topic;
     users*      users_list;			// Changer en liste d'ids des users
	 struct channel *next;
};

bool							channel_exists(std::string name, channel *channels);
channel *add_new_channel(channel *channels, channel *new_channel);
channel							*find_channel(std::string name, channel *channels);
channel							*create_channel(std::string name, std::string topic);
users							*add_user_channel(users *users_list, users* user);

#endif