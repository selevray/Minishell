/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_pids.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:43:20 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 11:58:52 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	gc_add_pid(t_gc *gc, pid_t pid)
{
	int	new_capacity;

	if (gc->pids.count == gc->pids.capacity)
	{
		new_capacity = gc->pids.capacity * 2;
		gc->pids.pids = ft_realloc(gc->pids.pids, gc->pids.capacity
				* sizeof(pid_t), new_capacity * sizeof(pid_t));
		if (gc->pids.pids == NULL)
			return ;
		gc->pids.capacity = new_capacity;
	}
	gc->pids.pids[gc->pids.count] = pid;
	gc->pids.count++;
}

int	gc_wait_pids(t_gc *gc)
{
	size_t	i;
	int		status;

	i = 0;
	status = 0;
	while (i < gc->pids.count)
	{
		waitpid(gc->pids.pids[i], &status, 0);
		i++;
	}
	gc->pids.count = 0;
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
