/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:18:29 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:10:35 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <unistd.h>

static int	is_valid_var_name(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(t_shell *shell, char *var_name)
{
	int	i;
	int	var_len;

	var_len = ft_strlen(var_name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var_name, var_len) == 0
			&& shell->envp[i][var_len] == '=')
		{
			while (shell->envp[i])
			{
				shell->envp[i] = shell->envp[i + 1];
				i++;
			}
			return ;
		}
		i++;
	}
}

int	builtin_unset(t_shell *shell, char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (!is_valid_var_name(av[i]))
		{
			write(2, "unset: invalid variable name\n", 30);
			i++;
			continue ;
		}
		remove_env_var(shell, av[i]);
		i++;
	}
	return (0);
}
