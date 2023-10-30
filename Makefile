NAME = ircserv

SRCS	=	main.cpp \
			src/Server/Server.cpp \
			src/Server/server_utils.cpp \
			src/Client/Client.cpp \
			src/Client/client_utils.cpp \
			src/Channel/Channel.cpp \
			src/Commands/handleChannelCommands.cpp \
			src/Commands/messages.cpp  \
			src/Commands/pingpong.cpp  \
			src/Commands/userInformationCommands.cpp  \
			src/Commands/notice.cpp  \
			src/Commands/handleOperatorChannelCommands.cpp \
			src/Commands/userCommands.cpp  \
			src/Commands/handleMode.cpp

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
	rm -f $(NAMEB)
	rm -rf bin

re: fclean all

.PHONY: all clean fclean re
