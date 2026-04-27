/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 13:57:01 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/13 17:08:06 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize_input(t_gc *gc, char *input)
{
	t_token	*token_list;
	t_token	*new_tok;

	token_list = NULL;
	new_tok = NULL;
	while (*input)
	{
		skip_spaces(&input);
		if (*input == '\0')
			break ;
		new_tok = get_operator_token(gc, &input);
		if (!new_tok)
			new_tok = get_word_token(gc, &input);
		if (new_tok)
			add_token_back(&token_list, new_tok);
	}
	add_token_back(&token_list, create_token(gc, TOK_EOF, NULL, 0));
	return (token_list);
}
