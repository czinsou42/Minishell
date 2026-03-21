/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/21 14:57:33 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_heredoc_words(t_token *word)
{
	char	*result;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (word && word->type == TOKEN_WORD)
	{
		tmp = ft_strjoin(result, word->str);
		free(result);
		if (!tmp)
			return (NULL);
		result = tmp;
		if (word->next && word->next->joined)
			word = word->next;
		else
			break ;
	}
	return (result);
}

char	*get_heredoc_delimiter(t_token *current)
{
	char	*result;
	char	*clean;

	result = join_heredoc_words(current->next);
	if (!result)
		return (NULL);
	clean = remove_quote(result);
	free(result);
	if (!clean)
		return (NULL);
	return (clean);
}

void	heredoc_sigint(int sig)
{
	(void)sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	g_exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
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
					print_syntax_error(current->next->str);
				else
					print_unexpected_token();
				return (-1);
			}
			if (handle_single_heredoc(current, cleanup) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
