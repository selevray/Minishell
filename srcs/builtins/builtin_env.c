/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:16:19 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:09:37 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"

int	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		write(1, shell->envp[i], ft_strlen(shell->envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
