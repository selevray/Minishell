/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:36:55 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:36:59 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <stdio.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal;

typedef struct sigaction		t_sigaction;

void							handler_sigint(int sig);
void							set_signals_interactive(void);
void							set_signals_parent_exec(void);
void							set_signals_child_exec(void);

#endif