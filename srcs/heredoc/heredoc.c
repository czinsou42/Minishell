/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/16 15:44:20 by czinsou          ###   ########.fr       */
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
		return (free(line), free(line_with_newline), free(buffer), NULL);
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
			return (NULL);
		if (new_buffer == buffer)
			break ;
		buffer = new_buffer;
	}
	return (buffer);
}

int	process_heredoc(t_token *tokens)
{
	t_token		*current;
	char		*delimiter;
	char		*content;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC && current->next != NULL)
		{
			delimiter = current->next->str;
			content = read_heredoc_content(delimiter);
			if (!content)
				return (-1);
			current->heredoc_content = content;
		}
		current = current->next;
	}
	return (0);
}
// int	process_heredoc(t_token *tokens)
// {
// 	t_token	*current;
// 	char	*delimiter;
// 	char	*line;
// 	char	*joined_buffer;
// 	char	*buffer;
// 	char	*line_with_newline;

// 	current = tokens;
// 	while (current)
// 	{
// 		if (current->type == TOKEN_REDIR_HEREDOC && current->next != NULL)
// 		{
// 			buffer = ft_strdup("");
// 			if (!buffer)
// 				return (-1);
// 			delimiter = current->next->str;
// 			while (1)
// 			{
// 				line = readline("> ");
// 				if (!line)
// 					return (free(buffer), -1);
// 				if (ft_strcmp(line, delimiter) == 0)
// 				{
// 					free(line);
// 					break ;
// 				}
// 				line_with_newline = ft_strjoin(line, "\n");
// 				if (!line_with_newline)
// 					return (free(buffer), free(line), -1);
// 				joined_buffer = ft_strjoin(buffer, line_with_newline);
// 				if (!joined_buffer)
// 					return (free(line), free(line_with_newline), free(buffer),
// 						-1);
// 				free(buffer);
// 				free(line_with_newline);
// 				buffer = joined_buffer;
// 				free(line);
// 			}
// 			current->heredoc_content = buffer;
// 		}
// 		current = current->next;
// 	}
// 	return (0);
// }
