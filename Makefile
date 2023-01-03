CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = ircserv
INCDIR	= -I ./include
SOURCES_DIR = src/

SOURCES_NAME = main.cpp\
			Channel.cpp\
			Client.cpp\
			Command1.cpp\
			Command2.cpp\
			Command3.cpp\
			Server.cpp\
			Util.cpp

SOURCES = $(addprefix $(SOURCES_DIR), $(SOURCES_NAME))

OBJECTS = $(SOURCES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJECTS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re