/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_match.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:31:24 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 09:53:43 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static void	split_pattern(t_gc *gc, char *pattern, char **dir_out,
		char **name_out)
{
	char	*slash;
	size_t	dir_len;

	slash = ft_strrchr(pattern, '/');
	if (slash)
	{
		dir_len = slash - pattern;
		*dir_out = gc_alloc(gc, dir_len + 1);
		ft_strlcpy(*dir_out, pattern, dir_len + 1);
		*name_out = slash + 1;
	}
	else
	{
		*dir_out = NULL;
		*name_out = pattern;
	}
}

static DIR	*open_match_dir(char *dir_path, size_t *dir_len)
{
	if (dir_path)
	{
		*dir_len = ft_strlen(dir_path);
		return (opendir(dir_path));
	}
	*dir_len = 0;
	return (opendir("."));
}

int	count_matches(t_gc *gc, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;
	char			*dir_path;
	char			*name_pat;

	split_pattern(gc, pattern, &dir_path, &name_pat);
	count = 0;
	if (dir_path)
		dir = opendir(dir_path);
	else
		dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && wildcard_match(name_pat, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static char	*build_entry_path(t_gc *gc, char *dir_path, size_t dir_len,
		char *name)
{
	size_t	name_len;
	char	*copy;

	name_len = ft_strlen(name);
	if (dir_path)
	{
		copy = gc_alloc(gc, dir_len + 1 + name_len + 1);
		ft_strlcpy(copy, dir_path, dir_len + 1);
		copy[dir_len] = '/';
		ft_strlcpy(copy + dir_len + 1, name, name_len + 1);
	}
	else
	{
		copy = gc_alloc(gc, name_len + 1);
		ft_strlcpy(copy, name, name_len + 1);
	}
	return (copy);
}

void	fill_matches(t_gc *gc, char *pattern, char **new_argv, int *j)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*dir_path;
	char			*name_pat;
	size_t			dir_len;

	split_pattern(gc, pattern, &dir_path, &name_pat);
	dir = open_match_dir(dir_path, &dir_len);
	if (!dir)
		return ;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && wildcard_match(name_pat, entry->d_name))
		{
			new_argv[*j] = build_entry_path(gc, dir_path, dir_len,
					entry->d_name);
			(*j)++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
}
