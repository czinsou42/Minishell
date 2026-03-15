/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/15 13:07:28 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	static int	calls = 0;

	(void)sig;
	g_exit_status = 130;
	if (calls++ == 0)
		write(STDOUT_FILENO, "\n", 1);
}

int	process_heredoc(t_token *tokens, t_cleanup *cleanup)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->next)
					fprintf(stderr,
						"minishell: syntax error near unexpected token '%s'\n",
						current->next->str);
				else
					fprintf(stderr,
						"minishell: syntax error unexpected token'newline'\n");
				return (-1);
			}
			if (handle_single_heredoc(current, cleanup) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}