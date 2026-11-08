# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/16 12:40:32 by kong              #+#    #+#              #
#    Updated: 2026/03/27 20:39:37 by kong             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and Flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Archive and Flags
AR = ar
ARFLAGS = rcs

SRC_DIR = src
BUILD_DIR = build
MINIRT_HEADER_DIR = include
MLX_DIR = minilibx-linux

# Source Files
# ! to replace with individual source files
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Executable
NAME = miniRT

# Header Files
MLXFLAGS = -L${MLX_DIR} -lmlx -L/usr/lib/X11 -lXext -lX11 -lm
INCLUDES = -I${MINIRT_HEADER_DIR} -I${MLX_DIR}

# OBJS = $(SRCS:.c=.o)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(MINIRT_HEADER_DIR)/mrt.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Default Target
all: $(NAME)

$(MLX_DIR)/libmlx.a: | $(MLX_DIR)
	$(MAKE) -C $(MLX_DIR)

$(MLX_DIR):
	git clone https://github.com/42Paris/minilibx-linux.git $(MLX_DIR)

# Build Executable
$(NAME): $(MLX_DIR)/libmlx.a $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(MLXFLAGS) -o $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re