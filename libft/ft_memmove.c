/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:42:36 by selevray          #+#    #+#             */
/*   Updated: 2026/02/26 21:42:40 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *destination, const void *source, size_t size)
{
	unsigned char	*src;
	unsigned char	*dst;
	size_t			i;

	if (!destination && !source)
		return (NULL);
	src = (unsigned char *)source;
	dst = (unsigned char *)destination;
	if (dst < src)
	{
		i = 0;
		while (i < size)
		{
			dst[i] = src[i];
			i++;
		}
	}
	else
		while (size--)
			dst[size] = src[size];
	return (dst);
}
