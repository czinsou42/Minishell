/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:37:06 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/20 13:46:30 by czinsou          ###   ########.fr       */
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
	void	*bt[10];
	char	**symbols;

	int size, i;
	// 🔥 TRACE : affiche l'appel
	fprintf(stderr, "DEBUG: cleanup_and_exit called with exit_code=%d\n",
		g_exit_status);
	// 🔥 OPTIONNEL : affichage de la stack
	size = backtrace(bt, 10);
	symbols = backtrace_symbols(bt, size);
	fprintf(stderr, "DEBUG STACK TRACE:\n");
	for (i = 0; i < size; i++)
		fprintf(stderr, "  %s\n", symbols[i]);
	free(symbols);
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
