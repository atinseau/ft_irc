
CC=clang++
EXTRA_FLAGS=-Wall -Werror -Wextra
FLAGS=-std=c++98 -g -D DEV=0
BUILD_DIR=.build
NAME=ircserv

COMMAND= $(addprefix command/, \
			command.cpp \
			utils.cpp \
			$(addprefix commands/, \
				help.cpp \
				nick.cpp \
				pass.cpp \
				user.cpp \
				info.cpp \
				msg/privmsg.cpp \
				msg/notice.cpp \
				msg/utils.cpp \
				join.cpp \
				part.cpp \
				mode.cpp \
				list.cpp \
				all.cpp \
				topic.cpp \
				invite.cpp \
				kick.cpp \
				names.cpp \
			) \
		)

SRC= $(addprefix src/, \
		utils/utils.cpp \
		socket/socket.cpp\
		client/client.cpp\
		client/utils.cpp \
		request/request.cpp \
		request/utils.cpp \
		response/response.cpp \
		channel/channel.cpp \
		channel/utils.cpp \
		channel/mode.cpp \
		$(COMMAND) \
	)

OBJ=$(SRC:src/%.cpp=$(BUILD_DIR)/%.o)

SERVER_SRCS= main.cpp
SERVER_NAME=ft_irc

$(NAME): $(OBJ)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(EXTRA_FLAGS) $(FLAGS) -c $< -o $@
	@echo "✅ $< is compiled"

all: $(NAME)
	@$(CC) $(EXTRA_FLAGS) $(FLAGS) $(OBJ) $(SERVER_SRCS) -o $(NAME)
	@echo "✅ $(NAME) is ready to start"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "🗑 Cleaning library"

fclean: clean
	@rm -rf $(BUILD_DIR)
	@rm -f $(NAME)
	@echo "🗑 Cleaning all files"

re: fclean all

run: all
	@./$(NAME) 10000 06112001

.DEFAULT_GOAL:=all