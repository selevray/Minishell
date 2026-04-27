/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:52:22 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:22:47 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static t_ast	*parse_subshell(t_gc *gc, t_token **tokens)
{
	t_ast	*node;

	node = make_node(gc, NODE_SUBSHELL);
	*tokens = (*tokens)->next;
	node->left = parse_logical(gc, tokens);
	*tokens = (*tokens)->next;
	return (node);
}

static t_cmd	*fill_cmd(t_gc *gc, t_token **tokens, t_cmd *cmd)
{
	t_redir	*redir;
	size_t	i;

	i = 0;
	redir = NULL;
	while ((*tokens != NULL && (*tokens)->type != TOK_EOF)
		&& ((*tokens)->type == TOK_WORD || is_redir((*tokens)->type)))
	{
		if (is_redir((*tokens)->type))
		{
			redir = parse_redirection(gc, tokens);
			redir->next = cmd->redirections;
			cmd->redirections = redir;
		}
		else
		{
			cmd->argv[i] = (*tokens)->value;
			*tokens = (*tokens)->next;
			i++;
		}
	}
	cmd->argv[i] = NULL;
	return (cmd);
}

static t_ast	*parse_simple_command(t_gc *gc, t_token **tokens)
{
	t_cmd	*cmd;
	t_ast	*node;
	size_t	count;

	count = count_cmd_words(*tokens);
	cmd = gc_alloc(gc, sizeof(t_cmd));
	if (cmd == NULL)
		ms_fatal(gc, "parse_simple_command", "malloc failed", 1);
	cmd->argv = gc_alloc(gc, sizeof(char *) * (count + 1));
	if (cmd->argv == NULL)
		ms_fatal(gc, "parse_simple_command", "malloc failed", 1);
	node = make_node(gc, NODE_CMD);
	cmd->redirections = NULL;
	cmd = fill_cmd(gc, tokens, cmd);
	node->cmd = cmd;
	return (node);
}

t_ast	*parse_command(t_gc *gc, t_token **tokens)
{
	t_ast	*result;

	if ((*tokens != NULL && (*tokens)->type != TOK_EOF)
		&& (*tokens)->type == TOK_LPAREN)
		result = parse_subshell(gc, tokens);
	else
		result = parse_simple_command(gc, tokens);
	return (result);
}
