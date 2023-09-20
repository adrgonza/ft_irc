NAME = ircserv

SRCS	=	main.cpp \
			src/Server/Server.cpp \
			src/Client/Client.cpp \
			src/Channel/Channel.cpp \
			src/client_utils.cpp \
			src/server_utils.cpp \
			src/commands/handleChannelCommands.cpp \
			src/commands/messages.cpp  \


OBJS = $(SRCS:%.cpp=bin/%.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3 -I include

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

bin/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
