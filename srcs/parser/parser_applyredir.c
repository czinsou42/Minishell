/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_applyredir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/19 14:13:52 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_redir_filename(t_token *current)
{
	char	*result;
	char	*tmp;
	t_token	*word;

	word = current->next;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (word && word->type == TOKEN_WORD)
	{
		tmp = ft_strjoin(result, word->str);
		free(result);
		if (!tmp)
			return (NULL);
		result = tmp;
		if (word->next && word->next->joined)
			word = word->next;
		else
			break ;
	}
	return (result);
}

static t_redir	*init_redir_node(t_token *current)
{
	t_redir	*redir;

	if (!current || !current->next || current->next->type != TOKEN_WORD)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token_to_redir_type(current->type);
	redir->file = get_redir_filename(current);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	redir->heredoc_fd = -1;
	redir->heredoc_content = NULL;
	return (redir);
}

static int	apply_heredoc_content(t_redir *redir, t_token *current)
{
	if (!redir || !current)
		return (0);
	if (current->type == TOKEN_REDIR_HEREDOC)
	{
		if (current->heredoc_content)
			redir->heredoc_content = ft_strdup(current->heredoc_content);
		else
			redir->heredoc_content = ft_strdup("");
		if (!redir->heredoc_content)
			return (0);
	}
	return (1);
}

static t_redir	*create_redir_node(t_token *current)
{
	t_redir	*redir;

	if (!current || !current->next || current->next->type != TOKEN_WORD)
	{
		if (current && current->next)
			print_parser_redir_error(current->next);
		else
			print_unexpected_token();
		return (NULL);
	}
	if (current->type != TOKEN_REDIR_HEREDOC)
		return (init_redir_node(current));
	redir = init_redir_node(current);
	if (!redir)
		return (NULL);
	redir->heredoc_fd = current->heredoc_fd;
	if (!apply_heredoc_content(redir, current))
		return (free(redir->file), free(redir), NULL);
	return (redir);
}

t_redir	*parse_redirections(t_token *tokens, int length)
{
	t_token	*current;
	t_redir	*redir;
	t_redir	*head;
	int		i;

	current = tokens;
	head = NULL;
	i = 0;
	while (current && i < length)
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
		current = current->next;
		i++;
	}
	return (head);
}
