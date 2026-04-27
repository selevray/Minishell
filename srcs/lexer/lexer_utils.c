/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:11:27 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/13 17:53:03 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Crée un nouveau noeud de token */
t_token	*create_token(t_gc *gc, t_token_type type, char *value, int quoted)
{
	t_token	*new_node;

	new_node = gc_alloc(gc, sizeof(t_token));
	if (!new_node)
		ms_fatal(gc, "create_token", "malloc failed", 1);
	new_node->type = type;
	new_node->value = value;
	new_node->quoted = quoted;
	new_node->next = NULL;
	return (new_node);
}

/* Ajoute un token à la fin de la liste */
void	add_token_back(t_token **head, t_token *new_node)
{
	t_token	*temp;

	if (!head || !new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

void	skip_spaces(char **input)
{
	while (**input == ' ' || **input == '\t')
		(*input)++;
}
