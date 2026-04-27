/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:16:51 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:08:58 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <stdlib.h>

int	builtin_exit(t_shell *shell, char **av)
{
	int	exit_code;

	(void)shell;
	exit_code = 0;
	if (av[1])
	{
		if (!ft_isdigit((unsigned char)av[1][0]))
		{
			write(2, "exit: numeric argument required\n", 32);
			return (2);
		}
		exit_code = ft_atoi(av[1]);
	}
	exit(exit_code);
	return (0);
}
