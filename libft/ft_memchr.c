/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:44:10 by selevray          #+#    #+#             */
/*   Updated: 2026/02/05 14:44:18 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stddef.h"

void	*ft_memchr(const void *memoryBlock, int searchedChar, size_t size)
{
	unsigned char	*mb;
	size_t			i;

	mb = (unsigned char *)memoryBlock;
	i = 0;
	while (i < size)
	{
		if (mb[i] == (unsigned char)searchedChar)
			return (&mb[i]);
		i++;
	}
	return (NULL);
}
