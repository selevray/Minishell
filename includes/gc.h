/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gujarry <gujarry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:03:28 by selevray          #+#    #+#             */
/*   Updated: 2026/04/06 10:39:16 by gujarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../libft/libft.h"

typedef struct s_arena_node
{
	void				*ptr;
	struct s_arena_node	*next;
}						t_arena_node;

typedef struct s_arena
{
	t_arena_node		*head;
}						t_arena;

typedef struct s_fd_tracker
{
	size_t				*fds;
	size_t				count;
	size_t				capacity;
}						t_fd_tracker;

typedef struct s_pid_tracker
{
	pid_t				*pids;
	size_t				count;
	size_t				capacity;
}						t_pid_tracker;

typedef struct s_gc
{
	t_arena arena;      // mémoire temporaire (reset par commande)
	t_arena persistent; // mémoire persistante (env, gc lui-même)
	t_fd_tracker		fds;
	t_pid_tracker		pids;
	int					exit_status;
}						t_gc;

// gc_alloc.c
void					*gc_alloc(t_gc *gc, size_t size);
void					*gc_alloc_persistent(t_gc *gc, size_t size);
void					free_arena(t_arena *arena);

// gc_lifecycle.c
void					gc_reset(t_gc *gc);
void					gc_destroy(t_gc *gc);
void					gc_init(t_gc *gc);

// gc_fd.c
void					gc_close_fds(t_gc *gc);
void					gc_add_fd(t_gc *gc, int fd);

// gc_pids.c
void					gc_wait_pids(t_gc *gc);
void					gc_add_pid(t_gc *gc, int fd);

#endif