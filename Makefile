Name = ircserv

SRC = Server.cpp main.cpp Channel.cpp Client.cpp monitor.cpp

HEADERS = Channel.hpp Client.hpp Server.hpp monitor.hpp

CC = c++ -Wall -Wextra -Werror  -fsanitize=address -g -std=c++98

OBJ = $(SRC:.cpp=.o)

all: $(Name)

$(Name): $(OBJ)
	$(CC) $(OBJ) -o $(Name)

%.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@


clean: 
	rm -rf $(OBJ)
fclean: clean
	rm -rf $(Name)

re: fclean all

.PHONY: all clean fclean re