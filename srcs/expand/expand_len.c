/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 15:50:10 by selevray          #+#    #+#             */
/*   Updated: 2026/04/23 13:39:20 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static void	handle_quote(char *in_quote, char c)
{
	if (c == '\'' || c == '"')
	{
		if (c == *in_quote)
			*in_quote = 0;
		else if (*in_quote == 0)
			*in_quote = c;
	}
}

void	fill_expanded(t_shell *shell, char *str, char *result)
{
	size_t	i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (str[i])
	{
		handle_quote(&in_quote, str[i]);
		if (str[i] == '\'' || str[i] == '"')
			;
		else if (in_quote == '\'')
			*result++ = str[i];
		else if (str[i] != '$')
			*result++ = str[i];
		else
		{
			i++;
			fill_var(shell, str, &i, &result);
			continue ;
		}
		i++;
	}
}

int	expanded_len(t_shell *shell, char *str)
{
	size_t	len;
	size_t	i;
	char	in_quote;

	i = 0;
	len = 0;
	in_quote = 0;
	while (str[i])
	{
		handle_quote(&in_quote, str[i]);
		if (str[i] == '\'' || str[i] == '"')
			;
		else if (in_quote == '\'')
			len++;
		else if (str[i] == '$')
		{
			i++;
			len += expanded_len_var(shell, str, &i);
			continue ;
		}
		else
			len++;
		i++;
	}
	return (len);
}
