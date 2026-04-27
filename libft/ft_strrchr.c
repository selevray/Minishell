/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:53:25 by selevray          #+#    #+#             */
/*   Updated: 2026/02/26 21:53:27 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *string, int searchedChar)
{
	int				i;
	unsigned char	c;

	c = (unsigned char)searchedChar;
	i = ft_strlen(string);
	while (i >= 0)
	{
		if ((unsigned char)string[i] == c)
			return ((char *)&string[i]);
		i--;
	}
	return (NULL);
}
