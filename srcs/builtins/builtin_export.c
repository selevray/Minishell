/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:17:34 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:08:30 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "envp.h"
#include "libft.h"

static int	is_valid_var_name(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*create_env_var(t_shell *shell, char *var)
{
	char	*new_var;
	int		len;

	len = ft_strlen(var);
	new_var = gc_alloc_persistent(shell->gc, len + 1);
	if (!new_var)
		return (NULL);
	ft_strcpy(new_var, var);
	return (new_var);
}

// static int	count_envp(char **envp)
// {
// 	int	count;

// 	count = 0;
// 	while (envp[count])
// 		count++;
// 	return (count);
// }

static void	add_or_update_env(t_shell *shell, char *var)
{
	char	*equal;
	int		i;
	int		var_len;

	equal = ft_strchr(var, '=');
	if (!equal)
		return ;
	var_len = equal - var;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, var_len) == 0
			&& shell->envp[i][var_len] == '=')
		{
			shell->envp[i] = create_env_var(shell, var);
			return ;
		}
		i++;
	}
	if (shell->env_count + 1 >= (int)shell->env_capacity)
		realloc_envp(shell);
	shell->envp[i] = create_env_var(shell, var);
	shell->envp[i + 1] = NULL;
	shell->env_count++;
}

int	builtin_export(t_shell *shell, char **av)
{
	int	i;

	if (!av[1])
	{
		write(1, "export: no arguments\n", 21);
		return (0);
	}
	i = 1;
	while (av[i])
	{
		if (!is_valid_var_name(av[i]))
		{
			write(2, "export: invalid variable name\n", 30);
			return (1);
		}
		add_or_update_env(shell, av[i]);
		i++;
	}
	return (0);
}
