/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paser.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 12:05:20 by selevray          #+#    #+#             */
/*   Updated: 2026/04/13 08:44:10 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*parse_logical(t_gc *gc, t_token **tokens)
{
	t_ast			*node;
	t_token_type	op;

	node = gc_alloc(gc, sizeof(t_ast));
	node->left = parse_pipeline(gc, tokens);
	if ((*tokens != NULL && (*tokens)->type != TOK_EOF)
		&& ((*tokens)->type == TOK_AND || (*tokens)->type == TOK_OR))
	{
		op = (*tokens)->type;
		*tokens = (*tokens)->next;
		node->right = parse_pipeline(gc, tokens);
		if (op == TOK_AND)
			node->type = NODE_AND;
		else
			node->type = NODE_OR;
		return (node);
	}
	return (node->left);
}

t_ast	*parse_pipeline(t_gc *gc, t_token **tokens)
{
	t_ast	*left;
	t_ast	*pipe_node;

	left = parse_command(gc, tokens);
	while ((*tokens != NULL && (*tokens)->type != TOK_EOF)
		&& (*tokens)->type == TOK_PIPE)
	{
		pipe_node = gc_alloc(gc, sizeof(t_ast));
		pipe_node->type = NODE_PIPE;
		pipe_node->left = left;
		*tokens = (*tokens)->next;
		pipe_node->right = parse_command(gc, tokens);
		left = pipe_node;
	}
	return (left);
}

