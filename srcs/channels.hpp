#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include "users.hpp"
# include "response.hpp"
# include <string>
# include <iostream>
# include <list>
# include <sys/poll.h>
#define DBG(vari) std::cerr<<#vari<<" = "<<(vari)<<std::endl;

struct channel
{
     std::string	name;
     std::string	topic;
     int			users_id[100];
     int			nb_users;
	struct channel	*next;
};

channel*	create_channel(std::string name, std::string topic);
channel*	add_new_channel(channel *channels, channel *new_channel);
bool		channel_exists(std::string name, channel *channels);
channel*	find_channel(std::string name, channel *channels);
void		display_channel_users(channel *channel);
int       find_channel_user(channel *chan, int user_id);
int       remove_user_from_channels(channel *channels, int user_id, pollfd *fds, std::string nickname, std::string username, std::string part_msg);

#endif
