/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/19 14:13:52 by lebertau         ###   ########.fr       */
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

int	count_tokens_word(t_token *token, int length)
{
	int		count;
	t_token	*current;
	int		i;

	i = 0;
	count = 0;
	current = token;
	while (current && i < length)
	{
		if (is_redirection(current))
		{
			current = current->next;
			i++;
			if (current && i < length)
			{
				current = current->next;
				i++;
				while (current && current->joined && i < length)
				{
					current = current->next;
					i++;
				}
			}
			continue ;
		}
		if (current->type == TOKEN_WORD)
		{
			count++;
			while (current->next && current->next->joined && i + 1 < length)
			{
				current = current->next;
				i++;
			}
		}
		current = current->next;
		i++;
	}
	return (count);
}

void	free_redirections(t_redir *redir)
{
	t_redir *tmp;

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

int	print_parser_redir_error(t_token *token)
{
	if (token->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell :syntax error near unexpected token `", 2);
		ft_putstr_fd(token->str, 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	return (0);
}

void	print_unexpected_token(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
		2);
}
