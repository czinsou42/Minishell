/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:22:37 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/20 17:42:35 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ast(t_node *node, int depth)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	while (i < depth)
	{
		printf("\t");
		i++;
	}
	if (node->type == NODE_COMMAND)
	{
		printf("[CMD: %s]", node->command->argv[0]);
		if (node->command->redirections)
			printf(" \t -> REDIR");
	}
	if (node->type == NODE_PIPE)
		printf("[PIPE]");
	else if (node->type == NODE_AND)
		printf("[AND]");
	else if (node->type == NODE_OR)
		printf("[OR]");
	printf("\n");
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}

// void	free_ast(t_node *node)
// {
// 	if (node == NULL)
// 		return ;
// 	free_ast(node->left);
// 	free_ast(node->right);
// 	if (node->command)
// 	{
// 		if (node->command->argv)
// 			ft_free_split(node->command->argv);
// 		if (node->command->redirections)
// 		{
// 			free_redirections(node->command->redirections);
// 		}
// 		free(node->command);
// 	}
// 	free(node);
// }

void free_ast(t_node *node, int free_commands)
{
    if (!node)
        return;
    free_ast(node->left, free_commands);
    free_ast(node->right, free_commands);
    if (node->type == NODE_COMMAND && free_commands)
        free_command(node->command);
    free(node);
}
