/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:29:17 by selevray          #+#    #+#             */
/*   Updated: 2026/04/22 10:56:42 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "fcntl.h"
#include "readline/readline.h"

static int	apply_redir_append(t_gc *gc, t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		ms_fatal(gc, "open", "failed open", 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	apply_redir_in(t_gc *gc, t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_RDONLY, 0644);
	if (fd == -1)
		ms_fatal(gc, "open", "failed open", 1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	apply_redir_out(t_gc *gc, t_redir *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_fatal(gc, "open", "failed open", 1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	exec_redir(t_gc *gc, t_redir *redir)
{
	while (redir != NULL)
	{
		if (redir->type == REDIR_APPEND)
		{
			apply_redir_append(gc, redir);
		}
		else if (redir->type == REDIR_HEREDOC)
		{
			apply_redir_heredoc(gc, redir);
		}
		else if (redir->type == REDIR_IN)
		{
			apply_redir_in(gc, redir);
		}
		else if (redir->type == REDIR_OUT)
		{
			apply_redir_out(gc, redir);
		}
		redir = redir->next;
	}
	return (0);
}
