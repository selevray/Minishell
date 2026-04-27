/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:02:45 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:08:03 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "expand.h"
#include "libft.h"
#include <unistd.h>

static char	*get_target_path(t_shell *shell, char **av)
{
	char	*path;

	path = 0;
	if (av[1] == NULL)
	{
		path = get_env_value(shell, "HOME", 4);
		if (path == NULL)
		{
			write(1, "HOME not set", 12);
			return (NULL);
		}
	}
	else if (ft_strcmp(av[1], "-") == 0)
	{
		path = get_env_value(shell, "OLDPWD", 6);
		if (path == NULL)
		{
			write(1, "OLDPWD not set", 14);
			return (NULL);
		}
	}
	else
		path = av[1];
	return (path);
}

void	update_oldpwd(t_shell *shell, char *old_pwd)
{
	char	*new_value;
	int		i;

	new_value = gc_alloc_persistent(shell->gc, ft_strlen("OLDPWD=")
			+ ft_strlen(old_pwd) + 1);
	if (!new_value)
		return ;
	new_value[0] = '\0';
	ft_strcat(new_value, "OLDPWD=");
	ft_strcat(new_value, old_pwd);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
		{
			shell->envp[i] = new_value;
			return ;
		}
		i++;
	}
}

void	update_pwd(t_shell *shell, char *new_pwd)
{
	char	*new_value;
	int		i;

	new_value = gc_alloc_persistent(shell->gc, ft_strlen("PWD=")
			+ ft_strlen(new_pwd) + 1);
	if (!new_value)
		return ;
	new_value[0] = '\0';
	ft_strcat(new_value, "PWD=");
	ft_strcat(new_value, new_pwd);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PWD=", 4) == 0)
		{
			shell->envp[i] = new_value;
			return ;
		}
		i++;
	}
}

int	builtin_cd(t_shell *shell, char **av)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	path = get_target_path(shell, av);
	if (path == NULL)
		return (1);
	old_pwd = get_env_value(shell, "PWD", 3);
	update_oldpwd(shell, old_pwd);
	if (chdir(path) == -1)
	{
		write(1, "cd: can't change to directory\n", 30);
		return (1);
	}
	new_pwd = gc_alloc(shell->gc, 4096);
	if (!new_pwd)
		return (1);
	getcwd(new_pwd, 4096);
	update_pwd(shell, new_pwd);
	return (0);
}
