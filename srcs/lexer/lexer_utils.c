/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:11:27 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/07 11:30:07 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// Crée un nouveau noeud de token
t_token *create_token(t_gc *gc, t_token_type type, char *value)
{
    t_token *new_node = gc_alloc(gc, sizeof(t_token));
    if (!new_node)
        return (NULL);
    new_node->type = type;
    new_node->value = value; // value doit aussi avoir été alloué via le gc !
    new_node->next = NULL;
    return (new_node);
}

// Ajoute un token à la fin de la liste
void add_token_back(t_token **head, t_token *new_node)
{
    t_token *temp;

    if (!head || !new_node)
        return;
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new_node;
}