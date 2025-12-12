/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:57:07 by amwahab           #+#    #+#             */
/*   Updated: 2025/10/24 18:31:01 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pipeline(t_node *node, char **envp)
{
	t_pipeline	*pipeline;
	// 1. extract cmd from node if node->left-type == CMD
	pipeline = extract_cmd(node);
	// 2. 
}