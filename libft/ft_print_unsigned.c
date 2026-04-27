/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 03:06:08 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:28:34 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_unsigned(unsigned int n)
{
	char	c;
	int		len;

	len = 0;
	if (n >= 10)
	{
		len = ft_print_unsigned(n / 10);
		if (len == -1)
			return (-1);
	}
	c = (n % 10) + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	return (len + 1);
}
