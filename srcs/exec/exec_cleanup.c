/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:37:06 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/16 15:43:07 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_all_heredoc_fds(t_token *tokens)
{
	t_token	*all_tokens;

	all_tokens = tokens;
	while (all_tokens)
	{
		if (all_tokens->heredoc_fd != -1)
		{
			close(all_tokens->heredoc_fd);
			all_tokens->heredoc_fd = -1;
		}
		all_tokens = all_tokens->next;
	}
}

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
	{
		if (cleanup->tokens->heredoc_fd != -1)
			close_all_heredoc_fds(cleanup->tokens);
		free_tokens(cleanup->tokens);
	}
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
