NAME = ircserv

#SRCS = ./src/main.cpp ./src/server/Server.cpp ./src/server/serverUtils.cpp \
		./src/gnrl/utils.cpp ./src/gnrl/Client.cpp ./src/gnrl/Channel.cpp \
		./src/commands/commands.cpp ./src/commands/privMessage.cpp ./src/commands/changeNickname.cpp \
		./src/commands/handleChannelCommands.cpp ./src/commands/invite.cpp ./src/commands/userInformation.cpp \
		./src/commands/pingpong.cpp ./src/commands/sendNotice.cpp
SRC = main

SERVER = Server \
		 serverUtils 

CMDS = commands \
	   privMessage \
	   changeNickname \
	   handleChannelCommands \
	   invite \
	   userInformation \
	   pingpong \
	   sendNotice \

GNRL = utils \
       Client \
	   Channel 

DSRVER = $(addprefix server/, $(SERVER))
DCMDS  = $(addprefix commands/, $(CMDS))
DGNRL = $(addprefix gnrl/, $(GNRL))

FILES = $(DSRVER) $(DCMDS) $(DGNRL) $(SRC)

DSRCS = $(addprefix src/, $(FILES))
SRCS = $(addsuffix .cpp, $(DSRCS))

OBJS = $(SRCS:.cpp=.o)

CXX = g++ -std=c++98 -Iinc
CXXFLAGS = -Wall -Wextra -Werror -Iinc

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
