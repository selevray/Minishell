/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 15:52:24 by selevray          #+#    #+#             */
/*   Updated: 2026/04/22 12:02:04 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static int	is_valid_id_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	fill_var(t_shell *shell, char *str, size_t *i, char **result)
{
	size_t	name_len;
	char	*env_value;
	char	*s;

	if (str[*i] == '?')
	{
		s = gc_itoa(shell->gc, shell->exit_status);
		*result += ft_strlcpy(*result, s, ft_strlen(s) + 1);
		(*i)++;
	}
	else
	{
		name_len = 0;
		while (str[*i + name_len] && is_valid_id_char(str[*i + name_len]))
			name_len++;
		env_value = get_env_value(shell, &str[*i], name_len);
		if (env_value != NULL)
			*result += ft_strlcpy(*result, env_value, ft_strlen(env_value) + 1);
		*i += name_len;
	}
}

size_t	expanded_len_var(t_shell *shell, char *str, size_t *i)
{
	size_t	len;
	size_t	name_len;
	char	*env_value;

	len = 0;
	name_len = 0;
	if (str[*i] == '?')
	{
		len += int_len(shell->exit_status);
		(*i)++;
	}
	else
	{
		while (str[*i + name_len] && is_valid_id_char(str[*i + name_len]))
			name_len++;
		env_value = get_env_value(shell, &str[*i], name_len);
		*i += name_len;
		if (env_value != NULL)
			len += ft_strlen(env_value);
	}
	return (len);
}
