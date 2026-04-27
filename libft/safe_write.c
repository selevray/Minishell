/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 22:03:37 by selevray          #+#    #+#             */
/*   Updated: 2026/02/26 23:19:03 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

ssize_t	ft_safe_write(int fd, const void *buf, size_t count)
{
	ssize_t	total;
	ssize_t	ret;

	if (fd < 0 || !buf || count == 0)
		return (-1);
	total = 0;
	while ((size_t)total < count)
	{
		ret = write(fd, buf + total, count - total);
		if (ret == -1)
			return (-1);
		total += ret;
	}
	return (total);
}
