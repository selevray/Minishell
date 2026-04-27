/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:03:20 by selevray          #+#    #+#             */
/*   Updated: 2026/04/02 13:55:00 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	*gc_alloc(t_gc *gc, size_t size)
{
	void			*ptr;
	t_arena_node	*node;

	ptr = malloc(size);
	if (ptr == NULL)
		return (NULL);
	node = malloc(sizeof(t_arena_node));
	if (node == NULL)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = gc->arena.head;
	gc->arena.head = node;
	return (ptr);
}

void	*gc_alloc_persistent(t_gc *gc, size_t size)
{
	void			*ptr;
	t_arena_node	*node;

	ptr = malloc(size);
	if (ptr == NULL)
		return (NULL);
	node = malloc(sizeof(t_arena_node));
	if (node == NULL)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = gc->persistent.head;
	gc->persistent.head = node;
	return (ptr);
}

void	free_arena(t_arena *arena)
{
	t_arena_node	*node;
	t_arena_node	*tmp;

	node = arena->head;
	while (node != NULL)
	{
		tmp = node->next;
		free(node->ptr);
		free(node);
		node = tmp;
	}
	arena->head = NULL;
}
