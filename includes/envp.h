/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 09:06:50 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 12:07:27 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

# include "parser.h"

void	copy_envp(t_shell *shell, char **envp);
void	realloc_envp(t_shell *shell);

#endif