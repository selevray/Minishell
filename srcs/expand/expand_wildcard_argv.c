/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_argv.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:32:06 by selevray          #+#    #+#             */
/*   Updated: 2026/04/23 11:32:28 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static int	count_new_len(t_gc *gc, char **argv)
{
	size_t	i;
	int		len;
	int		n;

	i = 0;
	len = 0;
	while (argv[i])
	{
		if (has_wildcard(argv[i]))
		{
			n = count_matches(gc, argv[i]);
			if (n == 0)
				len++;
			else
				len += n;
		}
		else
			len++;
		i++;
	}
	return (len);
}

static void	fill_argv_loop(t_shell *shell, char **argv,
				char **new_argv, int *j)
{
	size_t	i;
	int		before;

	i = 0;
	while (argv[i])
	{
		if (has_wildcard(argv[i]))
		{
			before = *j;
			fill_matches(shell->gc, argv[i], new_argv, j);
			if (*j == before)
				new_argv[(*j)++] = argv[i];
		}
		else
			new_argv[(*j)++] = argv[i];
		i++;
	}
}

char	**expand_wildcards(t_shell *shell, char **argv)
{
	int		len;
	int		j;
	char	**new_argv;

	len = count_new_len(shell->gc, argv);
	new_argv = gc_alloc(shell->gc, sizeof(char *) * (len + 1));
	if (new_argv == NULL)
		ms_fatal(shell->gc, "expand_wildcard", "malloc failed", 1);
	j = 0;
	fill_argv_loop(shell, argv, new_argv, &j);
	new_argv[j] = NULL;
	return (new_argv);
}
