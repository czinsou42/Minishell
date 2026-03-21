/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/21 13:49:48 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_syntax_error(t_token *next)
{
	if (next)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		ft_putstr_fd(next->str, 2);
		ft_putstr_fd("'\n", 2);
	}
	else
		ft_putstr_fd("minishell: syntax error unexpected token'newline'\n", 2);
}

char	*get_heredoc_delimiter(t_token *current)
{
	char	*result;
	char	*tmp;
	t_token	*word;

	word = current->next;
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
				print_heredoc_syntax_error(current->next);
				return (-1);
			}
			if (handle_single_heredoc(current, cleanup) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
