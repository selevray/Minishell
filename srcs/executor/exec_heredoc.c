/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 10:07:06 by selevray          #+#    #+#             */
/*   Updated: 2026/04/22 10:53:19 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "fcntl.h"
#include "readline/readline.h"

static int	fill_heredoc(t_redir *redir, int fd)
{
	char	*line;

	line = readline("> ");
	while (line != NULL)
	{
		if (strcmp(line, redir->target) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline("> ");
	}
	return (0);
}

int	apply_redir_heredoc(t_gc *gc, t_redir *redir)
{
	int	fd;

	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_fatal(gc, "open", "failed open", 1);
	fill_heredoc(redir, fd);
	close(fd);
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
		ms_fatal(gc, "open", "failed open", 1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink("/tmp/minishell_heredoc");
	return (0);
}
