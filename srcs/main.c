/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 10:53:11 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 12:06:42 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envp.h"
#include "executor.h"
#include "minishell.h"
#include "signals.h"
#include "stdio.h"
#include <readline/history.h>
#include <readline/readline.h>

static t_shell	*init_shell(t_gc *gc, char **envp)
{
	t_shell	*shell;

	gc_init(gc);
	shell = gc_alloc_persistent(gc, sizeof(t_shell));
	shell->gc = gc;
	copy_envp(shell, envp);
	shell->exit_status = 0;
	set_signals_interactive();
	return (shell);
}

static void	process_input(t_shell *shell, char *line)
{
	t_token	*tokens;
	t_ast	*ast;

	add_history(line);
	tokens = tokenize_input(shell->gc, line);
	ast = parse_logical(shell->gc, &tokens);
	set_signals_parent_exec();
	exec_ast(shell, ast);
	if (g_signal == SIGINT)
		shell->exit_status = 130;
	g_signal = 0;
	free(line);
	gc_reset(shell->gc);
	set_signals_interactive();
}

int	main(int argc, char const *argv[], char **envp)
{
	char	*line;
	t_gc	gc;
	t_shell	*shell;

	(void)argv;
	(void)argc;
	shell = init_shell(&gc, envp);
	line = readline("Minishell\n~ ");
	while (line != NULL)
	{
		process_input(shell, line);
		line = readline("Minishell\n~ ");
	}
	write(1, "exit\n", 5);
	gc_destroy(&gc);
	return (shell->exit_status);
}
