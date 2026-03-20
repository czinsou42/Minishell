/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/20 16:07:53 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(t_token *token)
{
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_HEREDOC
		|| token->type == TOKEN_REDIR_APPEND);
}

static t_token	*skip_redir_tokens(t_token *current, int *i, int length)
{
	current = current->next;
	(*i)++;
	if (current && *i < length)
	{
		current = current->next;
		(*i)++;
		while (current && current->joined && *i < length)
		{
			current = current->next;
			(*i)++;
		}
	}
	return (current);
}

static t_token	*skip_joined_words(t_token *current, int *i, int length)
{
	while (current->next && current->next->joined && *i + 1 < length)
	{
		current = current->next;
		(*i)++;
	}
	return (current);
}

int	count_tokens_word(t_token *token, int length)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (token && i < length)
	{
		if (is_redirection(token))
		{
			token = skip_redir_tokens(token, &i, length);
			continue ;
		}
		if (token->type == TOKEN_WORD)
		{
			count++;
			token = skip_joined_words(token, &i, length);
		}
		token = token->next;
		i++;
	}
	return (count);
}

void	free_redirections(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		if (redir->heredoc_content)
			free(redir->heredoc_content);
		free(redir);
		redir = tmp;
	}
}
