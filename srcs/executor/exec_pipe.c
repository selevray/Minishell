/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:18:07 by selevray          #+#    #+#             */
/*   Updated: 2026/04/21 10:53:09 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	exec_pipe_right(t_shell *shell, t_ast *ast, int *fd)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
		ms_fatal(shell->gc, "fork", "fork failed", 1);
	else if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);
		exec_ast(shell, ast->right);
		exit(shell->exit_status);
	}
	else if (pid2 > 0)
	{
		gc_add_pid(shell->gc, pid2);
		close(fd[0]);
		close(fd[1]);
	}
}

void	exec_pipe(t_shell *shell, t_ast *ast)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		ms_fatal(shell->gc, "pipe", "failed pipe", 1);
	pid = fork();
	if (pid == -1)
		ms_fatal(shell->gc, "fork", "fork failed", 1);
	else if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		exec_ast(shell, ast->left);
		exit(shell->exit_status);
	}
	else if (pid > 0)
	{
		exec_pipe_right(shell, ast, fd);
		gc_add_pid(shell->gc, pid);
		shell->exit_status = gc_wait_pids(shell->gc);
	}
}
