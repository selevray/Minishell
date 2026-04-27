/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 10:03:52 by selevray          #+#    #+#             */
/*   Updated: 2026/04/21 10:51:27 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	exec_and(t_shell *shell, t_ast *ast)
{
	exec_ast(shell, ast->left);
	if (shell->exit_status == 0)
		exec_ast(shell, ast->right);
}

void	exec_or(t_shell *shell, t_ast *ast)
{
	exec_ast(shell, ast->left);
	if (shell->exit_status != 0)
		exec_ast(shell, ast->right);
}
