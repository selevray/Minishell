/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 10:36:12 by selevray          #+#    #+#             */
/*   Updated: 2026/04/23 13:39:10 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	wildcard_match(char *pattern, char *name)
{
	if (*pattern == '\0' && *name == '\0')
		return (1);
	if (*pattern == '*')
	{
		while (*name != '\0')
		{
			if (wildcard_match(pattern + 1, name))
				return (1);
			name++;
		}
		return (wildcard_match(pattern + 1, name));
	}
	if (*pattern == *name)
	{
		if (wildcard_match(pattern + 1, name + 1))
			return (1);
	}
	return (0);
}

int	has_wildcard(char *str)
{
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}
