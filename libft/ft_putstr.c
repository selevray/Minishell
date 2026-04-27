/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 02:48:39 by selevray          #+#    #+#             */
/*   Updated: 2026/02/27 02:50:39 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *string)
{
	int	len;

	if (!string)
		string = "(null)";
	len = ft_strlen(string);
	ft_safe_write(1, string, len);
	return (len);
}
