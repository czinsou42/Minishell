/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/12 15:07:04 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_heredoc_line(char *delimiter, char *buffer)
{
	char	*line;
	char	*line_with_newline;
	char	*joined_buffer;

	line = readline("> ");
	if (!line)
		return (free(buffer), NULL);
	if (ft_strcmp(line, delimiter) == 0)
		return (free(line), buffer);
	line_with_newline = ft_strjoin(line, "\n");
	if (!line_with_newline)
		return (free(buffer), free(line), NULL);
	joined_buffer = ft_strjoin(buffer, line_with_newline);
	if (!joined_buffer)
		return (free(buffer), free(line_with_newline), free(line), NULL);
	free(buffer);
	free(line_with_newline);
	free(line);
	return (joined_buffer);
}

static char	*read_heredoc_content(char *delimiter)
{
	char	*buffer;
	char	*new_buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (1)
	{
		new_buffer = read_heredoc_line(delimiter, buffer);
		if (!new_buffer)
			return (free(buffer), NULL);
		if (new_buffer == buffer)
			break ;
		buffer = new_buffer;
	}
	return (buffer);
}

static void	heredoc_child(char *delimiter, int write_fd)
{
	char	*content;

	signal(SIGINT, SIG_DFL);
	content = read_heredoc_content(delimiter);
	if (content)
	{
		write(write_fd, content, ft_strlen(content));
		free(content);
	}
	close(write_fd);
	exit(0);
}

static int	handle_single_heredoc(t_token *token)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child(token->next->str, pipefd[1]);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		return (-1);
	}
	token->heredoc_content = pipefd[0];
	return (0);
}

int	process_heredoc(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				fprintf(stderr,
					"minishell: syntax error near unexpected token '%s'\n",
					current->next ? current->next->str : "newline");
				return (-1);
			}
			if (handle_single_heredoc(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

// int	process_heredoc(t_token *tokens)
// {
// 	t_token	*current;
// 	char	*delimiter;
// 	char	*content;

// 	current = tokens;
// 	while (current)
// 	{
// 		if (current->type == TOKEN_REDIR_HEREDOC)
// 		{
// 			if (!current->next || current->next->type != TOKEN_WORD)
// 			{
// 				fprintf(stderr,
// 					"minishell: syntax error near unexpected token '%s'\n",
// 					current->next ? current->next->str : "newline");
// 				return (-1);
// 			}
// 			delimiter = current->next->str;
// 			content = read_heredoc_content(delimiter);
// 			if (!content)
// 				return (-1);
// 			current->heredoc_content = content;
// 		}
// 		current = current->next;
// 	}
// 	return (0);
// }
