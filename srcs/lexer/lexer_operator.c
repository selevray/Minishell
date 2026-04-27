/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:23:19 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/13 17:08:44 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_double_op(t_gc *gc, char **input)
{
	if ((*input)[0] == '|' && (*input)[1] == '|')
	{
		*input += 2;
		return (create_token(gc, TOK_OR, NULL, 0));
	}
	else if ((*input)[0] == '&' && (*input)[1] == '&')
	{
		*input += 2;
		return (create_token(gc, TOK_AND, NULL, 0));
	}
	else if ((*input)[0] == '<' && (*input)[1] == '<')
	{
		*input += 2;
		return (create_token(gc, TOK_HEREDOC, NULL, 0));
	}
	else if ((*input)[0] == '>' && (*input)[1] == '>')
	{
		*input += 2;
		return (create_token(gc, TOK_REDIR_APPEND, NULL, 0));
	}
	else
		return (NULL);
}

static t_token	*get_single_op_b(t_gc *gc, char **input)
{
	if ((*input)[0] == '(')
	{
		*input += 1;
		return (create_token(gc, TOK_LPAREN, NULL, 0));
	}
	else if ((*input)[0] == ')')
	{
		*input += 1;
		return (create_token(gc, TOK_RPAREN, NULL, 0));
	}
	else
		return (NULL);
}

static t_token	*get_single_op(t_gc *gc, char **input)
{
	if ((*input)[0] == '|')
	{
		*input += 1;
		return (create_token(gc, TOK_PIPE, NULL, 0));
	}
	else if ((*input)[0] == '&')
	{
		*input += 1;
		return (create_token(gc, TOK_BACKGROUND, NULL, 0));
	}
	else if ((*input)[0] == '<')
	{
		*input += 1;
		return (create_token(gc, TOK_REDIR_IN, NULL, 0));
	}
	else if ((*input)[0] == '>')
	{
		*input += 1;
		return (create_token(gc, TOK_REDIR_OUT, NULL, 0));
	}
	else
		return (get_single_op_b(gc, input));
}

t_token	*get_operator_token(t_gc *gc, char **input)
{
	t_token	*tok;

	tok = get_double_op(gc, input);
	if (tok == NULL)
	{
		tok = get_single_op(gc, input);
	}
	return (tok);
}
