SRCS = srcs/main.cpp srcs/users.cpp srcs/utils.cpp srcs/response.cpp srcs/commands.cpp srcs/server.cpp srcs/check.cpp srcs/channels.cpp
ODIR = .obj
OBJS = $(addprefix $(ODIR)/, $(SRCS:.cpp=.o))
DEP = $(OBJS:.o=.d)
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS = $(FLAGS) -MMD
RM = rm -rf
NAME = ircserv

all: init $(NAME)

init:
	@mkdir -p .obj/srcs

-include $(DEP)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

$(ODIR)/%.o: %.cpp Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(DEP)
	$(RM) $(ODIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
