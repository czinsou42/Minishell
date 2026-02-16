/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_applyredir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/16 16:07:03 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirection(t_token *token)
{
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_HEREDOC
		|| token->type == TOKEN_REDIR_APPEND);
}

static t_redir	*create_redir_node(t_token *current, t_redir *head)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token_to_redir_type(current->type);
	if (current->next && current->next->type == TOKEN_WORD)
		redir->file = ft_strdup(current->next->str);
	else
	{
		free(redir);
		if (current->next)
			print_parser_redir_error(current->next);
		else
			print_unexpected_token();
		free_redirections(head);
		return (REDIR_ERROR);
	}
	redir->next = NULL;
	if (current->type == TOKEN_REDIR_HEREDOC)
		redir->heredoc_content = ft_strdup(current->heredoc_content);
	else
		redir->heredoc_content = NULL;
	return (redir);
}

t_redir	*parse_redirections(t_token *tokens, int length)
{
	t_token	*current;
	t_redir	*redir;
	t_redir	*head;

	current = tokens;
	head = NULL;
	while (current && length--)
	{
		if (is_redirection(current))
		{
			redir = create_redir_node(current, head);
			if (!redir || redir == REDIR_ERROR)
				return (redir);
			redir_add_back(&head, redir);
			current = current->next;
		}
		current = current->next;
	}
	return (head);
}
