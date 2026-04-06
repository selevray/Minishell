/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gujarry <gujarry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 14:32:42 by gujarry           #+#    #+#             */
/*   Updated: 2026/04/06 10:37:03 by gujarry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/lexer.h"

/* Verifie si le caractere est un separateur (espace ou operateur) */
int	is_delimiter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

/* Calcule la longueur du prochain token de type WORD */
int	get_word_len(char *str)
{
	int	i;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (str[i])
	{
		/* Si on croise une single quote et qu'on n'est pas dans des double quotes */
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote; /* On inverse l'etat (entre ou sort) */
			
		/* Si on croise une double quote et qu'on n'est pas dans des single quotes */
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
			
		/* Si on n'est dans AUCUNE quote et qu'on croise un separateur -> fin du mot */
		else if (!in_squote && !in_dquote && is_delimiter(str[i]))
			break ;
			
		i++;
	}
	
	/* (Optionnel) Gerer l'erreur des quotes non fermees */
	if (in_squote || in_dquote)
	{
		/* Exemple de gestion d'erreur plus tard */
	}
	
	return (i);
}
