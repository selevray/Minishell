/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:10:09 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:08:17 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else
		return (0);
}

int	exec_builtin(t_shell *shell, char *cmd, char **av)
{
	if (!av || !av[0])
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (builtin_echo(shell, av));
	else if (ft_strcmp(cmd, "cd") == 0)
		return (builtin_cd(shell, av));
	else if (ft_strcmp(cmd, "env") == 0)
		return (builtin_env(shell));
	else if (ft_strcmp(cmd, "exit") == 0)
		return (builtin_exit(shell, av));
	else if (ft_strcmp(cmd, "export") == 0)
		return (builtin_export(shell, av));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (builtin_pwd(shell));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (builtin_unset(shell, av));
	return (0);
}
