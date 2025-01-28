TARGET = ft_traceroute

OBJ_PATH = obj
SRC_PATH = src
INC_PATH = inc

HEADERS = -I ./$(INC_PATH)

CC = gcc
CFLAGS =  -Wall -Wextra -Werror -pedantic -std=c11 -D_POSIX_C_SOURCE=200112L -g -pthread

REMOVE = rm -rf

SRC =	main.c \
		utils.c \
		args.c \

OBJ = $(addprefix $(OBJ_PATH)/, $(SRC:.c=.o))

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(HEADERS) -o $@
	@echo "ft_traceroute is compiled!"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INC_PATH)/*.h
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

clean:
	@$(REMOVE) $(OBJ_PATH)
	@echo "ft_traceroute is cleaned!"

fclean:
	@$(REMOVE) $(OBJ_PATH)
	@$(REMOVE) $(TARGET)
	@echo "ft_traceroute is fcleaned!"

re: fclean all
