/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:32:42 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/20 12:52:36 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Verifie si le caractere est un separateur (espace ou operateur) */
int	is_delimiter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

/* Calcule la longueur du prochain token de type WORD */
int	get_word_len(char *str)
{
	int	i;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if (!in_squote && !in_dquote && is_delimiter(str[i]))
			break ;
		i++;
	}
	if (in_squote == 1 || in_dquote == 1)
		return (-1);
	return (i);
}

t_token	*get_word_token(t_gc *gc, char **input)
{
	int		len;
	char	*value;
	int		i;
	int		quoted;

	i = 0;
	quoted = 0;
	len = get_word_len(*input);
	while (i < len)
	{
		if ((*input)[i] == '\'' || (*input)[i] == '\"')
		{
			quoted = 1;
		}
		i++;
	}
	if (len == 0)
		return (NULL);
	if (len == -1)
		return (ms_error(gc, "syntax error",
				"unexpected EOF while looking for matching quote", 2), NULL);
	value = gc_alloc(gc, len + 1);
	ft_strlcpy(value, *input, len + 1);
	*input += len;
	return (create_token(gc, TOK_WORD, value, quoted));
}
