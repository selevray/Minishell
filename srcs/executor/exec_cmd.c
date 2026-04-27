/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:16:09 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 10:51:31 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtins.h"

static char	*find_path_env(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL)
		perror("Environment variables not found");
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*build_path(t_gc *gc, char *dir, char *cmd)
{
	char	*full_path;

	full_path = gc_alloc(gc, ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

static char	*find_valid_path(t_gc *gc, char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = build_path(gc, paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*get_path_cmd(t_gc *gc, char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*final_path;
	char	*copy;

	copy = gc_alloc(gc, ft_strlen(cmd) + 1);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			ft_strlcpy(copy, cmd, ft_strlen(cmd) + 1);
			return (copy);
		}
		return (NULL);
	}
	path_env = find_path_env(envp);
	if (!path_env)
		path_env = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";
	paths = gc_split(gc, path_env, ':');
	if (!paths)
		return (NULL);
	final_path = find_valid_path(gc, paths, cmd);
	return (final_path);
}
