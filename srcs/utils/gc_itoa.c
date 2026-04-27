/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:02:53 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:20:41 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "stddef.h"

int	int_len(long long num)
{
	int	len;

	if (num == 0)
		return (1);
	len = 0;
	if (num < 0)
	{
		len++;
		num = -num;
	}
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

char	*gc_itoa(t_gc *gc, int n)
{
	char		*str;
	int			len;
	long long	num;

	num = n;
	len = int_len(num);
	str = gc_alloc(gc, sizeof(char) * (len + 1));
	if (!str)
		ms_fatal(gc, "parse_simple_command", "malloc failed", 1);
	str[len] = '\0';
	if (num == 0)
		str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	str[len] = '\0';
	while (num > 0)
	{
		str[len - 1] = num % 10 + '0';
		num /= 10;
		len--;
	}
	return (str);
}
