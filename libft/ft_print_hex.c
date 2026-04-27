/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 03:03:26 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:30:33 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_hex(unsigned long n, int format)
{
	char	*base;
	int		count;

	if (format)
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	count = 0;
	if (n >= 16)
	{
		count = ft_print_hex(n / 16, format);
		if (count == -1)
			return (-1);
	}
	if (ft_safe_write(1, &base[n % 16], 1) == -1)
		return (-1);
	return (count + 1);
}
