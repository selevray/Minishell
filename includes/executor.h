/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:33:10 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:33:16 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "parser.h"

char	*get_path_cmd(t_gc *gc, char *cmd, char **envp);

void	exec_pipe(t_shell *shell, t_ast *ast);

int		apply_redir_heredoc(t_gc *gc, t_redir *redir);

int		exec_redir(t_gc *gc, t_redir *redir);
void	exec_subshell(t_shell *shell, t_ast *ast);

void	exec_or(t_shell *shell, t_ast *ast);
void	exec_and(t_shell *shell, t_ast *ast);

void	exec_ast(t_shell *shell, t_ast *ast);

#endif