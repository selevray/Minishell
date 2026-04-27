/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:08:03 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/24 14:40:47 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "libft.h"
#include "parser.h"

// int	parse_builtin_cmd(t_shell *shell, t_cmd *cmd)
// {
// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 		return (0);

// 	if (is_builtin(cmd->argv[0]))
// 	{
// 		shell->exit_status = exec_builtin(shell, cmd->argv[0], cmd->argv);
// 		return (1);
// 	}
// 	return (0);
// }