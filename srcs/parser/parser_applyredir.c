/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_applyredir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/19 11:35:58 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*create_redir_node(t_token *current)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token_to_redir_type(current->type);
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		if (current->next)
			print_parser_redir_error(current->next);
		else
			print_unexpected_token();
		return (free(redir), NULL);
	}
	redir->file = ft_strdup(current->next->str);
	if (!redir->file)
		return (free(redir), NULL);
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
			redir = create_redir_node(current);
			if (!redir)
			{
				free_redirections(head);
				return (NULL);
			}
			redir_add_back(&head, redir);
			current = current->next;
		}
		current = current->next;
	}
	return (head);
}
