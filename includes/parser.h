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
	char *target; // filename ou heredoc delimiter
	struct s_redir	*next;
}					t_redir;

typedef enum e_node_type
{
	NODE_CMD,     // feuille : une commande simple (argv + redirections)
	NODE_PIPE,    // branche : pipe entre deux sous-arbres
	NODE_AND,     // branche : && entre deux sous-arbres (bonus)
	NODE_OR,      // branche : || entre deux sous-arbres (bonus)
	NODE_SUBSHELL // branche : (...) sous-shell, un seul enfant (bonus)
}					t_node_type;

typedef struct s_cmd
{
	char **argv;           // arguments (argv[0] = commande)
	t_redir *redirections; // liste chaînée de redirections
}					t_cmd;

typedef struct s_ast
{
	t_node_type		type;
	t_cmd *cmd;          // seulement si type == NODE_CMD
	struct s_ast *left;  // enfant gauche (PIPE, AND, la OR) ou unique enfant (SUBSHELL)
	struct s_ast *right; // enfant droit  (PIPE, AND, OR) — NULL pour SUBSHELL
}					t_ast;
#endif // PARSER_H