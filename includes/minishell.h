/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:33:43 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:33:44 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "gc.h"
# include "lexer.h"
# include "parser.h"

void	ms_error(t_gc *gc, char *context, char *msg, int status);
void	ms_fatal(t_gc *gc, char *context, char *msg, int status);

char	**gc_split(t_gc *gc, char const *str, char charset);
char	*gc_itoa(t_gc *gc, int n);
int		int_len(long long num);
#endif