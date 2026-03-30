/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 15:03:28 by selevray          #+#    #+#             */
/*   Updated: 2026/03/30 15:07:30 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include <stddef.h>
# include <unistd.h>
# include <sys/types.h>

typedef struct s_arena_node
{
    void                  *ptr;
    struct s_arena_node   *next;
} t_arena_node;

typedef struct s_arena
{
    t_arena_node  *head;
} t_arena;

typedef struct s_fd_tracker
{
    int   *fds;
    int   count;
    int   capacity;
} t_fd_tracker;

typedef struct s_pid_tracker
{
    pid_t *pids;
    int   count;
    int   capacity;
} t_pid_tracker;

typedef struct s_gc
{
    t_arena        arena;       // mémoire temporaire (reset par commande)
    t_arena        persistent;  // mémoire persistante (env, gc lui-même)
    t_fd_tracker   fds;
    t_pid_tracker  pids;
    int            exit_status;
} t_gc;

#endif