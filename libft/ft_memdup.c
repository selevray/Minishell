/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:45:03 by selevray          #+#    #+#             */
/*   Updated: 2026/02/06 13:24:20 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

void	*memdup(const void *mem, size_t size)
{
	unsigned char	*tab;
	size_t			i;

	tab = malloc(size);
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		tab[i] = ((unsigned char *)mem)[i];
		i++;
	}
	return (tab);
}
