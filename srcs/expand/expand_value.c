/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 10:56:10 by selevray          #+#    #+#             */
/*   Updated: 2026/04/22 13:01:06 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

char	*get_env_value(t_shell *shell, char *name, size_t len)
{
	int	i;

	i = 0;
	while (shell->envp != NULL && shell->envp[i])
	{
		if ((strncmp(shell->envp[i], name, len) == 0)
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*expand_value(t_shell *shell, char *str)
{
	size_t	len;
	char	*result;

	len = expanded_len(shell, str);
	result = gc_alloc(shell->gc, len + 1);
	fill_expanded(shell, str, result);
	result[len] = '\0';
	return (result);
}
