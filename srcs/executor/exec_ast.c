/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:03:26 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 11:52:36 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "expand.h"
#include "signals.h"

static char	**expand_argv(t_shell *shell, char **argv)
{
	int		j;
	char	**new_argv;

	j = 0;
	while (argv[j])
	{
		argv[j] = expand_value(shell, argv[j]);
		j++;
	}
	new_argv = expand_wildcards(shell, argv);
	return (new_argv);
}

static int	handle_builtin(t_shell *shell, t_ast *ast)
{
	ast->cmd->argv = expand_argv(shell, ast->cmd->argv);
	exec_redir(shell->gc, ast->cmd->redirections);
	return (exec_builtin(shell, ast->cmd->argv[0], ast->cmd->argv));
}

static void	handle_external(t_shell *shell, t_ast *ast)
{
	char	*path;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ms_fatal(shell->gc, "fork", "fork failed", 1);
	else if (pid == 0)
	{
		set_signals_child_exec();
		ast->cmd->argv = expand_argv(shell, ast->cmd->argv);
		path = get_path_cmd(shell->gc, ast->cmd->argv[0], shell->envp);
		exec_redir(shell->gc, ast->cmd->redirections);
		execve(path, ast->cmd->argv, shell->envp);
		ms_fatal(shell->gc, "execve", "failed execve", 1);
	}
	else if (pid > 0)
	{
		gc_add_pid(shell->gc, pid);
		shell->exit_status = gc_wait_pids(shell->gc);
	}
}

static void	exec_simple_cmd(t_shell *shell, t_ast *ast)
{
	set_signals_parent_exec();
	if (is_builtin(ast->cmd->argv[0]))
	{
		shell->exit_status = handle_builtin(shell, ast);
	}
	else
	{
		handle_external(shell, ast);
	}
}

void	exec_ast(t_shell *shell, t_ast *ast)
{
	if (ast == NULL)
		return ;
	else if (ast->type == NODE_AND)
	{
		exec_and(shell, ast);
	}
	else if (ast->type == NODE_OR)
	{
		exec_or(shell, ast);
	}
	else if (ast->type == NODE_CMD)
	{
		exec_simple_cmd(shell, ast);
	}
	else if (ast->type == NODE_PIPE)
	{
		exec_pipe(shell, ast);
	}
	else if (ast->type == NODE_SUBSHELL)
	{
		exec_subshell(shell, ast);
	}
}
