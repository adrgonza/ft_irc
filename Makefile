NAME = ircserv

SRCS = ./src/main.cpp ./src/server/Server.cpp \
		./src/gnrl/utils.cpp ./src/gnrl/Client.cpp \
		./src/commands/commands.cpp ./src/commands/privMessage.cpp ./src/commands/changeNickname.cpp \
		./src/commands/handleChannelCommands.cpp ./src/commands/invite.cpp

OBJS = $(SRCS:.cpp=.o)

CXX = g++ -std=c++98
CXXFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
