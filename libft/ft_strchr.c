/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:48:13 by selevray          #+#    #+#             */
/*   Updated: 2026/02/26 21:48:15 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *string, int searchedChar)
{
	size_t			i;
	unsigned char	c;

	c = (unsigned char)searchedChar;
	i = 0;
	while (i <= ft_strlen(string))
	{
		if ((unsigned char)string[i] == c)
			return ((char *)&string[i]);
		i++;
	}
	return (NULL);
}
