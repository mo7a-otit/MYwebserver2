NAME = webserv

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

SRCS = srcs/main.cpp\
		srcs/webserver.cpp\
		srcs/Location.cpp\
		srcs/ServerConfig.cpp\
		srcs/Client.cpp\
		srcs/Request.cpp\

OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
			c++ $(CXXFLAGS) $(OBJS) -o $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all