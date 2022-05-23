
CC=clang++
FLAGS=-Wall -Werror -Wextra -std=c++98 -g

COMMAND=	src/command/command.cpp \
			src/command/exception.cpp

LIB=	src/utils/utils.cpp \
		src/socket/socket.cpp\
		src/client/client.cpp\
		src/request/request.cpp \
		src/response/response.cpp \
		src/channel/channel.cpp \
		$(COMMAND)


OBJS_LIB=$(LIB:.cpp=.o)

SERVER_SRCS= main.cpp
SERVER_NAME=ft_irc

%.o:%.cpp
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "Compiling $<"

$(SERVER_NAME): $(OBJS_LIB)

all: $(SERVER_NAME)
	@$(CC) $(FLAGS) $(OBJS_LIB) $(SERVER_SRCS) -o $(SERVER_NAME) -lpthread
	@echo "Compiling server"

clean:
	@rm -f $(OBJS_LIB)
	@echo "Cleaning library"

fclean: clean
	@rm -f $(SERVER_NAME)
	@rm -rf build
	@echo "Cleaning build folder"

re: fclean all


.DEFAULT_GOAL:=all