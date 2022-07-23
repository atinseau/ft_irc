
CC=clang++
EXTRA_FLAGS=-Wall -Werror -Wextra
FLAGS=-std=c++98 -g

COMMAND= $(addprefix command/, \
			command.cpp \
			$(addprefix commands/, \
				help.cpp \
				nick.cpp \
				pass.cpp \
				user.cpp \
				info.cpp \
				privmsg.cpp \
				quit.cpp \
				join.cpp \
			) \
		)

LIB= $(addprefix src/, \
		utils/utils.cpp \
		socket/socket.cpp\
		client/client.cpp\
		client/utils.cpp \
		request/request.cpp \
		request/utils.cpp \
		response/response.cpp \
		channel/channel.cpp \
		$(COMMAND) \
	)

OBJS_LIB=$(LIB:.cpp=.o)

SERVER_SRCS= main.cpp
SERVER_NAME=ft_irc

%.o:%.cpp
	@$(CC) $(FLAGS) -c  $< -o $@
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

run: all
	@./$(SERVER_NAME) 10000 06112001

.DEFAULT_GOAL:=all