/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 13:57:01 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/07 11:22:29 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

t_token *tokenize_input(t_gc *gc, char *input)
{
    t_token *token_list = NULL;
    t_token *new_tok = NULL;

    while (*input)
    {
        skip_spaces(&input);
        if (*input == '\0')
            break;

        // Essayer de lire un opérateur
        new_tok = get_operator_token(&input);
        
        // Si ce n'est pas un opérateur, c'est forcément un mot
        if (!new_tok)
            new_tok = get_word_token(&input);

        // Ajouter à la liste
        if (new_tok)
            add_token_back(&token_list, new_tok);
    }
    
    // Optionnel : Ajouter un token EOF à la fin
    add_token_back(&token_list, create_token(TOK_EOF, NULL));
    
    return (token_list);
}
