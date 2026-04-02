/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main_gc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 14:05:52 by selevray          #+#    #+#             */
/*   Updated: 2026/04/02 14:23:41 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	t_gc gc;
	int fd;
	pid_t pid;
	int i;

	// 1. Init
	gc_init(&gc);

	// 2. 10 allocations de tailles variées
	i = 0;
	while (i < 10)
	{
		gc_alloc(&gc, 16);
		i++;
	}

	// 3. Ouvrir 3 fds et les tracker
	fd = open("/dev/null", O_RDONLY);
	gc_add_fd(&gc, fd);
	fd = open("/dev/null", O_RDONLY);
	gc_add_fd(&gc, fd);
	fd = open("/dev/null", O_RDONLY);
	gc_add_fd(&gc, fd);

	// 4. Fork + tracker le pid
	pid = fork();
	if (pid == 0)
	{
		gc_destroy(&gc);
		exit(0); // enfant sort immédiatement
	}
	gc_add_pid(&gc, pid);

	// 5. Attendre + reset + destroy
	gc_wait_pids(&gc);
	gc_reset(&gc);
	gc_destroy(&gc);

	printf("GC test passed\n");
	return (0);
}