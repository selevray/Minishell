/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:42:22 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 11:51:31 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp.h"

void	copy_envp(t_shell *shell, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	shell->env_count = i;
	shell->env_capacity = i + 16;
	shell->envp = gc_alloc_persistent(shell->gc, sizeof(char *)
			* shell->env_capacity);
	i = 0;
	while (i < shell->env_count)
	{
		shell->envp[i] = gc_alloc_persistent(shell->gc, ft_strlen(envp[i]) + 1);
		ft_strcpy(shell->envp[i], envp[i]);
		i++;
	}
	shell->envp[shell->env_count] = NULL;
}

void	realloc_envp(t_shell *shell)
{
	int		new_capacity;
	char	**new_envp;
	int		i;

	i = 0;
	new_capacity = shell->env_capacity * 2;
	new_envp = gc_alloc_persistent(shell->gc, sizeof(char *) * new_capacity);
	while (i < shell->env_count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[shell->env_count] = NULL;
	shell->envp = new_envp;
	shell->env_capacity = new_capacity;
}
