/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:15:38 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 12:08:21 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <unistd.h>

static int	parse_flag(char **av, int *i)
{
	int	newline;

	newline = 1;
	*i = 1;
	while (av[*i] && av[*i][0] == '-' && av[*i][1] == 'n')
	{
		if (av[*i][2] != '\0')
			break ;
		newline = 0;
		(*i)++;
	}
	return (newline);
}

static void	print_argument(char **av, int i)
{
	while (av[i])
	{
		write(1, av[i], ft_strlen(av[i]));
		if (av[i + 1])
			write(1, " ", 1);
		i++;
	}
}

int	builtin_echo(t_shell *shell, char **av)
{
	int	newline;
	int	i;

	(void)shell;
	newline = parse_flag(av, &i);
	print_argument(av, i);
	if (newline)
		write(1, "\n", 1);
	return (0);
}
