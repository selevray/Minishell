/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:33:01 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:33:20 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "executor.h"
# include <dirent.h>
# include <sys/types.h>

size_t	expanded_len_var(t_shell *shell, char *str, size_t *i);
int		expanded_len(t_shell *shell, char *str);

void	fill_var(t_shell *shell, char *str, size_t *i, char **result);
void	fill_expanded(t_shell *shell, char *str, char *result);

char	*get_env_value(t_shell *shell, char *name, size_t len);
char	*expand_value(t_shell *shell, char *str);

int		count_matches(t_gc *gc, char *pattern);
void	fill_matches(t_gc *gc, char *pattern, char **new_argv, int *j);

int		wildcard_match(char *pattern, char *name);
int		has_wildcard(char *str);

char	**expand_wildcards(t_shell *shell, char **argv);

#endif