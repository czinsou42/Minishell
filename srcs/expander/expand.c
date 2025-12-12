/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:28:47 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/19 08:40:52 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expander(t_token *tokens, char **envp)
{
	t_token	*current;
	char	*expanded_str;
	char	*expanded_heredoc;

	current = tokens;
	while (current)
	{
		if (current->quote != SINGLE_QUOTE)
		{
			expanded_str = expand(current->str, envp);
			if (current->type == TOKEN_REDIR_HEREDOC && current->heredoc_content)
			{
				expanded_heredoc = expand(current->heredoc_content, envp);
				free(current->heredoc_content);
				current->heredoc_content = expanded_heredoc;
			}
			free(current->str);
			current->str = expanded_str;
		}
		current = current->next;
	}
	return ;
}

char *expand(char *str, char **envp)
{
	int		total_len;
	char	*expanded_str;
	char	*result;

	total_len = expanded_len(str, envp);
	expanded_str = malloc(sizeof(char) * total_len + 1);
	if (!expanded_str)
		return (NULL);
	result = copy_expanded_str(str, expanded_str, envp);
	if (!result)
	{
		free(expanded_str);
		return(NULL);
	}
	return (expanded_str);
}

char *ft_getenv(char *name, char **envp)
{
	int i;
	int len;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
