/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:48:51 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/27 11:10:09 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "parser.h"

int		builtin_echo(t_shell *shell, char **av);

void	update_oldpwd(t_shell *shell, char *old_pwd);
void	update_pwd(t_shell *shell, char *new_pwd);
int		builtin_cd(t_shell *shell, char **av);

int		builtin_pwd(t_shell *shell);

int		builtin_export(t_shell *shell, char **av);

int		builtin_unset(t_shell *shell, char **av);

int		builtin_env(t_shell *shell);

int		builtin_exit(t_shell *shell, char **av);

int		is_builtin(char *cmd);
int		exec_builtin(t_shell *shell, char *cmd, char **av);

#endif