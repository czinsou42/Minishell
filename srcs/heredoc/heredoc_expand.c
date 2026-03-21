/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 14:33:00 by lebertau          #+#    #+#             */
/*   Updated: 2026/03/21 14:22:55 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_line_if_needed(char *line, char **envp, int should_expand)
{
	t_expand_tokens	expand_tokens;
	char			*expanded;

	if (!should_expand)
		return (line);
	expand_tokens.str = line;
	expand_tokens.envp = envp;
	expanded = expand(&expand_tokens);
	if (!expanded)
		return (line);
	free(line);
	return (expanded);
}

static char	*read_heredoc_line(char *delimiter, char *buffer, char **envp,
		int should_expand)
{
	char	*line;
	char	*expanded_line;
	char	*line_with_newline;
	char	*joined_buffer;

	line = readline("> ");
	if (g_exit_status == 130 || !line)
		return (free(line), free(buffer), NULL);
	if (ft_strcmp(line, delimiter) == 0)
		return (free(line), buffer);
	expanded_line = expand_line_if_needed(line, envp, should_expand);
	line_with_newline = ft_strjoin(expanded_line, "\n");
	if (!line_with_newline)
		return (free(buffer), free(expanded_line), NULL);
	joined_buffer = ft_strjoin(buffer, line_with_newline);
	if (!joined_buffer)
		return (free(buffer), free(line_with_newline),
			free(expanded_line), NULL);
	free(buffer);
	free(line_with_newline);
	free(expanded_line);
	return (joined_buffer);
}

char	*read_heredoc_content(char *delimiter, char **envp, int should_expand)
{
	char	*buffer;
	char	*new_buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (1)
	{
		new_buffer = read_heredoc_line(delimiter, buffer, envp, should_expand);
		if (!new_buffer)
			return (NULL);
		if (new_buffer == buffer)
			break ;
		buffer = new_buffer;
	}
	return (buffer);
}

int	should_expand_heredoc(t_token *token)
{
	t_token	*word;

	word = token->next;
	while (word && word->type == TOKEN_WORD)
	{
		if (word->quote != NO_QUOTE)
			return (0);
		if (word->next && word->next->joined)
			word = word->next;
		else
			break ;
	}
	return (1);
}
