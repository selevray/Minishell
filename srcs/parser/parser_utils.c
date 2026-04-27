/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:53:04 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:23:24 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	count_tok(t_token *tokens, t_token_type type)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp != NULL && tmp->type != TOK_EOF && tmp->type == type)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int	is_redir(t_token_type tokens)
{
	if (tokens == TOK_REDIR_APPEND || tokens == TOK_HEREDOC
		|| tokens == TOK_REDIR_IN || tokens == TOK_REDIR_OUT)
		return (1);
	return (0);
}

size_t	count_cmd_words(t_token *tokens)
{
	size_t	count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp != NULL && tmp->type != TOK_EOF && (tmp->type == TOK_WORD
			|| is_redir(tmp->type)))
	{
		if (tmp->type == TOK_WORD)
		{
			tmp = tmp->next;
			count++;
		}
		else
		{
			tmp = tmp->next;
			tmp = tmp->next;
		}
	}
	return (count);
}

t_redir_type	convert_redir_type(t_token_type type)
{
	t_redir_type	redir;

	if (type == TOK_REDIR_IN)
		redir = REDIR_IN;
	else if (type == TOK_REDIR_OUT)
		redir = REDIR_OUT;
	else if (type == TOK_REDIR_APPEND)
		redir = REDIR_APPEND;
	else
		redir = REDIR_HEREDOC;
	return (redir);
}

t_redir	*parse_redirection(t_gc *gc, t_token **tokens)
{
	t_redir	*redirection;

	redirection = gc_alloc(gc, sizeof(t_redir));
	redirection->type = convert_redir_type((*tokens)->type);
	(*tokens) = (*tokens)->next;
	redirection->target = (*tokens)->value;
	(*tokens) = (*tokens)->next;
	redirection->next = NULL;
	return (redirection);
}
