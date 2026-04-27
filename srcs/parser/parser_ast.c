/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 12:53:26 by selevray          #+#    #+#             */
/*   Updated: 2026/04/24 14:22:11 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast	*make_node(t_gc *gc, t_node_type type)
{
	t_ast	*ast;

	ast = gc_alloc(gc, sizeof(t_ast));
	if (ast == NULL)
		ms_fatal(gc, "Make node", "malloc failed", 1);
	ast->type = type;
	ast->cmd = NULL;
	ast->left = NULL;
	ast->right = NULL;
	return (ast);
}
