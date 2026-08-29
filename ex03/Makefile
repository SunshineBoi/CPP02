# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/23 14:18:51 by kong              #+#    #+#              #
#    Updated: 2026/08/28 16:11:06 by kong             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and Flags
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

# Archive and Flags
AR = ar
ARFLAGS = rcs

SRC_DIR = .
BUILD_DIR = build
HEADER_DIR = includes

# Source Files
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Executable
NAME = fixed

# Header Files
# INCLUDES = -I${HEADER_DIR}

# OBJS = $(SRCS:.c=.o)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Default Target
all: $(NAME)

# Build Executable
$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re