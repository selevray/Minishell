/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:46:39 by selevray          #+#    #+#             */
/*   Updated: 2026/04/20 10:08:50 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*extract_word(t_gc *gc, char const *s, char c)
{
	char	*word;
	int		len;
	int		i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = (char *)gc_alloc(gc, sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**fill_split(t_gc *gc, char const *s, char c, char **result)
{
	int	i;
	int	words;

	i = 0;
	words = count_words(s, c);
	while (i < words)
	{
		while (*s == c)
			s++;
		result[i] = extract_word(gc, s, c);
		while (*s && *s != c)
			s++;
		i++;
	}
	result[i] = NULL;
	return (result);
}

char	**gc_split(t_gc *gc, char const *str, char charset)
{
	char	**result;
	int		words;

	if (!str)
		return (NULL);
	words = count_words(str, charset);
	result = (char **)gc_alloc(gc, sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	return (fill_split(gc, str, charset, result));
}
