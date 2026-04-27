/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:18:01 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:09:49 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "expand.h"
#include <unistd.h>

int	builtin_pwd(t_shell *shell)
{
	char	*pwd;

	pwd = get_env_value(shell, "PWD", 3);
	if (pwd == NULL)
	{
		write(2, "pwd: PWD not set\n", 17);
		return (1);
	}
	write(1, pwd, ft_strlen(pwd));
	write(1, "\n", 1);
	return (0);
}
