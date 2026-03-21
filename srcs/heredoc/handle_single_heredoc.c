/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:28:24 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/21 14:22:29 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child(char *delimiter, int write_fd, t_cleanup *cleanup,
		int expand_content)
{
	char	*content;

	g_exit_status = 0;
	setup_heredoc_signals();
	signal(SIGQUIT, SIG_IGN);
	content = read_heredoc_content(delimiter, cleanup->envp, expand_content);
	free(delimiter);
	if (!content || g_exit_status == 130)
	{
		close(write_fd);
		rl_clear_history();
		cleanup_and_exit(cleanup, 130);
	}
	if (write(write_fd, content, ft_strlen(content)) == -1)
		perror("minishell: write");
	free(content);
	close(write_fd);
	rl_clear_history();
	cleanup_and_exit(cleanup, 0);
}

static int	fork_heredoc_child(t_token *token, int *pipefd,
t_cleanup *cleanup)
{
	pid_t	pid;
	char	*delimiter;
	int		expand_content;

	expand_content = should_expand_heredoc(token);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		delimiter = get_heredoc_delimiter(token);
		heredoc_child(delimiter, pipefd[1], cleanup, expand_content);
	}
	return (pid);
}

int	handle_single_heredoc(t_token *token, t_cleanup *cleanup)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	token->heredoc_fd = -1;
	token->heredoc_content = NULL;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), -1);
	signal(SIGINT, SIG_IGN);
	pid = fork_heredoc_child(token, pipefd, cleanup);
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, handler_signal);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipefd[0]), (g_exit_status = 130), -1);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	if (g_exit_status == 130)
		return (close(pipefd[0]), -1);
	token->heredoc_fd = pipefd[0];
	return (0);
}
