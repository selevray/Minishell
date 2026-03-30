/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:48:51 by selevray          #+#    #+#             */
/*   Updated: 2026/02/26 21:48:53 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = ft_strlen(s);
	dup = malloc(i + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, i + 1);
	return (dup);
}
