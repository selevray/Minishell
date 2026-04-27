/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 10:02:31 by selevray          #+#    #+#             */
/*   Updated: 2026/04/21 10:49:59 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	exec_subshell(t_shell *shell, t_ast *ast)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ms_fatal(shell->gc, "fork", "fork failed", 1);
	else if (pid == 0)
	{
		exec_ast(shell, ast->left);
		exit(shell->exit_status);
	}
	else if (pid > 0)
	{
		gc_add_pid(shell->gc, pid);
		shell->exit_status = gc_wait_pids(shell->gc);
	}
}
