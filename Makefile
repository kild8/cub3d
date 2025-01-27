# Compiler
CC = gcc

# Executable name
NAME = cub3d

# Directories
SRC_DIR = kiki
OBJ_DIR = obj
MLX_DIR = mlx

# Sources
SRC = 	$(SRC_DIR)/main.c \
		$(SRC_DIR)/player.c \
		$(SRC_DIR)/texture.c
OBJ =	$(OBJ_DIR)/main.o \
		$(OBJ_DIR)/player.o \
		$(OBJ_DIR)/texture.o

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -I$(MLX_DIR)

# Linker flags
LDFLAGS = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm

# Targets
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

# Object files compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
