/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:32:05 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 09:05:26 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "gc.h"
# include "lexer.h"

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}					t_redir;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}					t_node_type;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirections;
}					t_cmd;

typedef struct s_ast
{
	t_node_type		type;
	t_cmd			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct s_shell
{
	t_gc			*gc;
	char			**envp;
	int				exit_status;
	int				env_count;
	int				env_capacity;
}					t_shell;

size_t				count_tok(t_token *tokens, t_token_type type);
size_t				count_cmd_words(t_token *tokens);
t_redir_type		convert_redir_type(t_token_type type);
int					is_redir(t_token_type tokens);
t_redir				*parse_redirection(t_gc *gc, t_token **tokens);

t_ast				*make_node(t_gc *gc, t_node_type type);

t_ast				*parse_command(t_gc *gc, t_token **tokens);

t_ast				*parse_logical(t_gc *gc, t_token **tokens);
t_ast				*parse_pipeline(t_gc *gc, t_token **tokens);

#endif