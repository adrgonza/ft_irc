NAME = ircserv
NAMEB = ircbot

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
			src/commands/handleMode.cpp

BOTSRCS = src/Bot/Bot.cpp src/Bot/bot_utils.cpp src/Bot/BotClass.cpp

OBJS = $(SRCS:%.cpp=bin/%.o)

BOTOBJS = $(BOTSRCS:%.cpp=bin/%.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I include -fsanitize=address -g3
LDFLAGS = -lcurl

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

bonus: $(BOTOBJS)
		$(CXX) $(CXXFLAGS) -o $(NAMEB) $(BOTOBJS) $(LDFLAGS)

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
