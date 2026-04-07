/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:19:02 by selevray          #+#    #+#             */
/*   Updated: 2026/04/07 09:29:42 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_error(t_gc *gc, char *context, char *msg, int status)
{
    write(2, "minishell: ", 11);
	write(2, context, ft_strlen(context));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
    gc->exit_status = status;
}
void	ms_fatal(t_gc *gc, char *context, char *msg, int status)
{
    write(2, "minishell: ", 11);
	write(2, context, ft_strlen(context));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	gc_destroy(gc);
    exit(status % 256);
}