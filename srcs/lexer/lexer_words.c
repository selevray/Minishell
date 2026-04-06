/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gujarry <gujarry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:32:42 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/03 14:33:10 by gujarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

// Vérifie si le caractère est un séparateur (espace ou opérateur)
bool is_delimiter(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return true;
    if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')')
        return true;
    return false;
}

// Calcule la longueur du prochain token de type WORD
int get_word_len(char *str)
{
    int i = 0;
    bool in_squote = false; // '
    bool in_dquote = false; // "

    while (str[i])
    {
        // Si on croise une single quote et qu'on n'est pas dans des double quotes
        if (str[i] == '\'' && !in_dquote)
            in_squote = !in_squote; // On inverse l'état (entre ou sort)
            
        // Si on croise une double quote et qu'on n'est pas dans des single quotes
        else if (str[i] == '\"' && !in_squote)
            in_dquote = !in_dquote;
            
        // Si on n'est dans AUCUNE quote et qu'on croise un séparateur -> c'est la fin du mot
        else if (!in_squote && !in_dquote && is_delimiter(str[i]))
            break;
            
        i++;
    }
    
    // (Optionnel pour l'instant) Gérer l'erreur des quotes non fermées
    if (in_squote || in_dquote)
    {
        // Dans Minishell, si la ligne finit avec une quote ouverte, c'est une erreur de syntaxe
        // write(2, "Erreur: quotes non fermées\n", 28);
        // return (-1); 
    }
    
    return i;
}