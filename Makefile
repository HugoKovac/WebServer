NAME=WebServer
CC=g++
FLAGS= -Wall -Werror -Wextra -std=c++98 -g -O0

SRC= main.cpp server.cpp Request.cpp accept_connection.cpp \
	handle_request.cpp fileLocation.cpp parseConfig.cpp host.cpp \
	send.cpp utils.cpp listen.cpp parseConfig_insert.cpp \
	parseConfig_utils.cpp parseConfig_common.cpp \
	common.cpp GET.cpp redirect.cpp parse_uri.cpp \
	php-cgi.cpp DELETE.cpp exec_cgi.cpp  POST.cpp error_generator.cpp \

SRCS=$(addprefix srcs/, $(SRC))
INC_DIR=includes
OBJS=${SRC:.cpp=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS): $(SRCS)
	$(CC)  $(FLAGS) -I $(INC_DIR) $(SRCS) -c

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY:all, re, clean, fclean
