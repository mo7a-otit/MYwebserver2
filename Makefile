NAME = webserv
SRCS = Server/main.cpp \
		Server/ServerManager.cpp \
		config_Post/Location.cpp \
		config_Post/Config.cpp \
		config_Post/PostContentLength.cpp \
		Request/Request.cpp

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
CXX = c++
OBJ = $(SRCS:.cpp=.o)
all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all 