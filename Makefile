# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: selevray <selevray@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/30 14:50:38 by selevray          #+#    #+#              #
#    Updated: 2026/03/30 15:30:37 by selevray         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#  VARIABLES

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I includes -I libft

RM			= rm -rf

#  DIRECTORIES

SRC_DIR		= src
OBJ_DIR		= objs
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

#  SOURCE FILES — prefixed by module

# ── GC (Arena Collector) 
SRC_GC		= 

# ── Lexer 
SRC_LEXER	= 

# ── Parser (AST) 
SRC_PARSER	= 

# ── Expander 
SRC_EXPAND	= 

# ── Executor 
SRC_EXEC	= 

# ── Builtins 
SRC_BUILTIN	= 

# ── Environment
SRC_ENV		= 

# ── Signals
SRC_SIGNAL	= 

# ── Utils 
SRC_UTILS	= 

# ── Main 
SRC_MAIN	= ms_main.c

#  AGGREGATE

SRCS		= $(SRC_GC) \
			  $(SRC_LEXER) \
			  $(SRC_PARSER) \
			  $(SRC_EXPAND) \
			  $(SRC_EXEC) \
			  $(SRC_BUILTIN) \
			  $(SRC_ENV) \
			  $(SRC_SIGNAL) \
			  $(SRC_UTILS) \
			  $(SRC_MAIN)

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

#  COLORS

GREEN		= \033[0;32m
YELLOW		= \033[0;33m
CYAN		= \033[0;36m
RED			= \033[0;31m
RESET		= \033[0m

#  RULES

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(CYAN)  compiling $(YELLOW)$<$(RESET)"

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJ_DIR)
	@echo "$(RED)✗ object files removed$(RESET)"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(RESET)"

re: fclean all

debug: CFLAGS += -g3 -fsanitize=address
debug: re
	@echo "$(YELLOW)⚠ compiled with -g3 -fsanitize=address (don't use with valgrind)$(RESET)"

#  PHONY

.PHONY: all clean fclean re debug