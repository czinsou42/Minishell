/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:37:06 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/20 14:43:54 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <execinfo.h> 

static void	free_pipeline_list(t_pipeline *head)
{
	t_pipeline	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

void	cleanup_and_exit(t_cleanup *cleanup, int status)
{
	if (cleanup->line)
		free(cleanup->line);
	if (cleanup->pipeline)
	{
		free_pipeline_list(cleanup->head_pipeline);
		cleanup->pipeline = NULL;
		cleanup->head_pipeline = NULL;
	}
	if (cleanup->tokens)
		free_tokens(cleanup->tokens);
	if (cleanup->ast)
		free_ast(cleanup->ast);
	free_envp(cleanup->envp);
	clear_history();
	exit(status);
}

void	cleanup_iteration(t_cleanup *cleanup)
{
	if (cleanup->tokens)
		free_tokens(cleanup->tokens);
	if (cleanup->ast)
		free_ast(cleanup->ast);
	cleanup->tokens = NULL;
	cleanup->ast = NULL;
}
