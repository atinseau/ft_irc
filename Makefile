
CC=clang++
FLAGS=-Wall -Werror -Wextra -std=c++98

LIB=src/socket.cpp
OBJS_LIB=$(LIB:.cpp=.o)

SERVER_SRCS= server.cpp
SERVER_NAME=build/server

CLIENT_SRCS= client/main.cpp
CLIENT_NAME=build/client

%.o:%.cpp
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "Compiling library"

$(SERVER_NAME): $(OBJS_LIB)
	@mkdir -p build
	@echo "Mounting build folder"

all: $(SERVER_NAME)
	@$(CC) $(FLAGS) $(OBJS_LIB) $(SERVER_SRCS) -o $(SERVER_NAME) -lpthread
	@echo "Compiling server"

clean:
	@rm -f $(OBJS_LIB)
	@echo "Cleaning library"

fclean: clean
	@rm -f $(SERVER_NAME)
	@rm -f $(CLIENT_NAME)
	@rm -rf build
	@echo "Cleaning build folder"

re: fclean all

# OPTIONNEL
client: $(OBJS_LIB)
	@mkdir -p build
	@$(CC) $(FLAGS) $(OBJS_LIB) $(CLIENT_SRCS) -o $(CLIENT_NAME) -lpthread
	@./$(CLIENT_NAME)

.DEFAULT_GOAL:=all