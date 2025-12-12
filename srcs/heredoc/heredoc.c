/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/19 10:43:20 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc(t_token *tokens)
{
	t_token	*current;
	char	*delimiter;
	char	*line;
	char	*joined_buffer;
	char	*buffer;
	char	*line_with_newline;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC && current->next != NULL)
		{
			buffer = ft_strdup("");
			if (!buffer)
				return (-1);
			delimiter = current->next->str;
			while (1)
			{
				line = readline("> ");
				if (!line)
					return(free(buffer), -1);
				if (ft_strcmp(line, delimiter) == 0)
				{
					free(line);
					break ;
				}
				line_with_newline = ft_strjoin(line, "\n");
				if (!line_with_newline)
					return (free(buffer), free(line), -1);
				joined_buffer = ft_strjoin(buffer, line_with_newline);
				if (!joined_buffer)
					return (free(line), free(line_with_newline), free(buffer), -1);
				free(buffer);
				free(line_with_newline);
				buffer = joined_buffer;
				free(line);
			}
			current->heredoc_content = buffer;
		}
		current = current -> next;
	}
	return (0);
}

