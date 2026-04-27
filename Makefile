# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: selevray <selevray@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/22 00:31:37 by selevray          #+#    #+#              #
#    Updated: 2026/04/27 11:11:22 by selevray         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#  VARIABLES

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -Wpedantic
INCLUDES	= -I includes -I libft

RM			= rm -rf

#  DIRECTORIES

SRC_DIR		= srcs
OBJ_DIR		= objs
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

#  SOURCE FILES — prefixed by module

# ── GC (Arena Collector)
SRC_GC		= gc/gc_alloc.c \
			  gc/gc_fd.c \
			  gc/gc_lifecycle.c \
			  gc/gc_pids.c

# ── Lexer
SRC_LEXER	= lexer/lexer.c \
			  lexer/lexer_operator.c \
			  lexer/lexer_utils.c \
			  lexer/lexer_words.c

# ── Parser (AST)
SRC_PARSER	= parser/parser_ast.c \
			  parser/parser_cmd.c \
			  parser/parser_utils.c \
			  parser/parser.c \
			  parser/parser_builtins.c

# ── Expander

SRC_EXPAND	= expand/expand_value.c \
			  expand/expand_len.c \
			  expand/expand_var.c \
			  expand/expand_wildcard.c \
			  expand/expand_wildcard_argv.c \
			  expand/expand_wildcard_match.c \
			  

# ── Executor
SRC_EXEC	= executor/exec_ast.c \
			  executor/exec_cmd.c \
			  executor/exec_pipe.c \
			  executor/exec_redir.c \
			  executor/exec_and_or.c \
			  executor/exec_subshell.c \
			  executor/exec_heredoc.c \

# ── Builtins
SRC_BUILTIN	= builtins/builtin_echo.c \
			  builtins/builtin_cd.c \
			  builtins/builtin_pwd.c \
			  builtins/builtin_export.c \
			  builtins/builtin_unset.c \
			  builtins/builtin_env.c \
			  builtins/builtin_exit.c \
			  builtins/builtin_dispatch.c

# ── Environment
SRC_ENV		= env/envp.c \

# ── Signals
SRC_SIGNAL	= signals/signals.c

# ── Utils
SRC_UTILS	= utils/error.c \
			  utils/gc_split.c \
			  utils/gc_itoa.c \

# ── Main
SRC_MAIN	= main.c

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

test_gc: $(LIBFT)
	@$(CC) $(CFLAGS) $(INCLUDES) \
		$(addprefix $(SRC_DIR)/, $(SRC_GC) gc/test_main_gc.c) \
		-L$(LIBFT_DIR) -lft -o test_gc
	@echo "$(GREEN)✓ test_gc compiled$(RESET)"

test_lexer: $(LIBFT)
	@$(CC) $(CFLAGS) $(INCLUDES) \
		$(addprefix $(SRC_DIR)/, $(SRC_GC) $(SRC_LEXER) $(SRC_UTILS) lexer/test_lexer.c) \
		-L$(LIBFT_DIR) -lft -o test_lexer
	@echo "$(GREEN)✓ test_lexer compiled$(RESET)"

test_parser: $(LIBFT)
	@$(CC) $(CFLAGS) $(INCLUDES) \
		$(addprefix $(SRC_DIR)/, $(SRC_GC) $(SRC_LEXER) $(SRC_PARSER) $(SRC_UTILS) parser/test_parser.c) \
		-L$(LIBFT_DIR) -lft -o test_parser
	@echo "$(GREEN)✓ test_parser compiled$(RESET)"

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJ_DIR)
	@echo "$(RED)✗ object files removed$(RESET)"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)
	@echo "$(RED)✗ $(NAME) removed$(RESET)"

re: fclean all

debug: CFLAGS += -g3 -fsanitize=address -Wlpedantic
debug: re
	@echo "$(YELLOW)⚠ compiled with -g3 -fsanitize=address (don't use with valgrind)$(RESET)"

#  PHONY

.PHONY: all clean fclean re debug test_gc test_lexer test_parser