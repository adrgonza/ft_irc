NAME = ircserv

SRCS	=	main.cpp \
			src/Server/Server.cpp \
			src/Server/server_utils.cpp \
			src/Client/Client.cpp \
			src/Client/client_utils.cpp \
			src/Channel/Channel.cpp \
			src/commands/handleChannelCommands.cpp \
			src/commands/messages.cpp  \
			src/commands/pingpong.cpp  \
			src/commands/userInformationCommands.cpp  \
			src/commands/notice.cpp  \
			src/commands/handleOperatorChannelCommands.cpp \
			src/commands/userCommands.cpp  \


OBJS = $(SRCS:%.cpp=bin/%.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I include -fsanitize=address -g3

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
	rm -rf bin

re: fclean all

.PHONY: all clean fclean re
