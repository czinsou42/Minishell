/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:50:17 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/19 09:03:36 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_ast(t_node *node, char **envp, t_cleanup *cleanup)
{
	int	status;

	status = 0;
	if (node == NULL)
		return (-1);
	if (node->type == NODE_COMMAND)
		status = exec_command(node->command, envp, cleanup);
	else if (node->type == NODE_AND)
		status = exec_and(node, envp, cleanup);
	else if (node->type == NODE_OR)
		status = exec_or(node, envp, cleanup);
	/*else if (node->type == NODE_PIPE)
		status = exec_pipeline(node, envp);*/
	return (status);
}
