/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_lifecycle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:53:12 by selevray          #+#    #+#             */
/*   Updated: 2026/04/02 14:03:18 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	gc_init(t_gc *gc)
{
	gc->arena.head = NULL;
	gc->persistent.head = NULL;
	gc->fds.count = 0;
	gc->fds.capacity = 8;
	gc->fds.fds = malloc(gc->fds.capacity * sizeof(int));
	if (gc->fds.fds == NULL)
		return ;
	gc->pids.count = 0;
	gc->pids.capacity = 8;
	gc->pids.pids = malloc(gc->pids.capacity * sizeof(pid_t));
	if (gc->pids.pids == NULL)
		return ;
	gc->exit_status = 0;
}
void	gc_reset(t_gc *gc)
{
    free_arena(&gc->arena);
    gc_close_fds(gc);      // ← ferme tous les fds
    gc->pids.count = 0;    // ← clear les pids (déjà waités avant)
}

void	gc_destroy(t_gc *gc)
{
    free_arena(&gc->arena);
    free_arena(&gc->persistent);
    gc_close_fds(gc);      // ← ferme les fds restants
    free(gc->fds.fds);     // ← libère le tableau lui-même
    free(gc->pids.pids);   // ← libère le tableau lui-même
}
