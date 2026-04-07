/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:23:19 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/07 11:29:56 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

// char *ft_strndup(const char *s, int n)
// {
//     int i = 0;
//     char *dup;

//     dup = malloc(sizeof(char) * (n + 1));
//     if (!dup)
//         return (NULL);
//     while (i < n && s[i])
//     {
//         dup[i] = s[i];
//         i++;
//     }
//     dup[i] = '\0';
//     return (dup);
// }

t_token *get_word_token(t_gc *gc, char **input)
{
    int len;
    char *value;
    
    // 1. Calculer la longueur exacte du mot (en ignorant les séparateurs entre quotes)
    len = get_word_len(*input);
    
    if (len == 0) // Sécurité
        return NULL;
    // if (len == -1) -> gérer le cas d'erreur de quotes non fermées si tu l'as implémenté
        
    // 2. Extraire la chaîne
    value = gc_alloc(gc, len + 1);
    ft_strlcpy(value, *input, len);
    
    // 3. Avancer le pointeur principal de la chaîne d'entrée
    *input += len;
    
    // 4. Créer et retourner le token
    return (create_token(gc, TOK_WORD, value));
}

