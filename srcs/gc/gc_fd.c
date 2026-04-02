/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_fd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:41:39 by selevray          #+#    #+#             */
/*   Updated: 2026/04/02 13:57:00 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	gc_add_fd(t_gc *gc, int fd)
{
	int new_capacity;

	if (gc->fds.count == gc->fds.capacity)
	{
		new_capacity = gc->fds.capacity * 2;
		gc->fds.fds = ft_realloc(gc->fds.fds, gc->fds.capacity * sizeof(int),
				new_capacity * sizeof(int));
		if (gc->fds.fds == NULL)
			return ;
		gc->fds.capacity = new_capacity;
	}
	gc->fds.fds[gc->fds.count] = fd;
	gc->fds.count++;
}

void gc_close_fds(t_gc *gc)
{
	size_t i;

	i = 0;
	while (i < gc->fds.count)
	{
		close(gc->fds.fds[i]);
		i++;
	}
	gc->fds.count = 0;
}