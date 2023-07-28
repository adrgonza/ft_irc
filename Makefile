NAME = ircserv

SRCS	=	main.cpp \
			src/Server.cpp \
			src/Client.cpp \
			src/client_utils.cpp \
			src/server_utils.cpp

OBJS = $(SRCS:%.cpp=bin/%.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I include

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
