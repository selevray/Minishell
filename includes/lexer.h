/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:29:04 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:29:50 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "gc.h"
# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC,
	TOK_WILDCARD,
	TOK_BACKGROUND,
	TOK_EOF
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				quoted;
	struct s_token	*next;
}					t_token;

t_token				*create_token(t_gc *gc, t_token_type type, char *value,
						int quoted);
void				add_token_back(t_token **head, t_token *new_node);
void				skip_spaces(char **input);

int					get_word_len(char *str);
t_token				*get_word_token(t_gc *gc, char **input);
int					is_delimiter(char c);

t_token				*get_operator_token(t_gc *gc, char **input);

t_token				*tokenize_input(t_gc *gc, char *input);

#endif