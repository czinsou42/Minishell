/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/19 08:46:19 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_add_back(t_redir **lst, t_redir *new)
{
	t_redir	*current;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

char	**create_argv(t_token *tokens, int count, int length)
{
	char	**argv;
	int		i;
	t_token	*current;

	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	current = tokens;
	while (current && i < length)
	{
		if (current->type == TOKEN_LPAREN || current->type == TOKEN_RPAREN)
			return (argv[i] = NULL, ft_putstr_fd
				("minishell: syntax error near unexpected token `('\n", 2),
				ft_free_split(argv), NULL);
		if (current->type == TOKEN_REDIR_APPEND || current->type == TOKEN_REDIR_HEREDOC || current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT)
		{
			current = current->next;
			if (current)
				current = current->next;
			continue ;
		}
		if (current->type == TOKEN_WORD)
		{
			argv[i++] = ft_strdup(current->str);
			if (!argv[i - 1])
				return (ft_free_split(argv), NULL);
		}
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
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
		if (current->type == TOKEN_WORD)
			count++;
		current = current->next;
		i++;
	}
	return (count);
}

t_redir	*parse_redirections(t_token *tokens, int length)
{
	t_token	*current;
	t_redir	*redir;
	t_redir	*head_redir;

	current = tokens;
	head_redir = NULL;
	while (current && length--)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_HEREDOC|| current->type == TOKEN_REDIR_APPEND)
		{
			redir = malloc(sizeof(t_redir));
			if (!redir)
				return (NULL);
			redir->type = token_to_redir_type(current->type);
			if (current->next && current->next->type == TOKEN_WORD) 					 // cas où il y a un fichier après l'opérateur "ls > infile.txt"
				redir->file = ft_strdup(current->next->str);
			else if (current->next)														 // cas où ce n'est pas un fichier après "ls > >"
			{
				print_parser_redir_error(current->next);
				return (free(redir), free_redirections(head_redir), REDIR_ERROR);
			}
			else																		 // cas où il n'y a pas de token après l'opérateur "ls >"
				return (free(redir), free_redirections(head_redir), print_unexpected_token(), REDIR_ERROR);
			redir->next = NULL;
			if (current->type == TOKEN_REDIR_HEREDOC)
				redir->heredoc_content = ft_strdup(current->heredoc_content);
			redir_add_back(&head_redir, redir);
			current = current->next;
		}
		current = current->next;
	}
	return (head_redir);
}

void	free_redirections(t_redir *redirections)
{
	t_redir	*current;
	t_redir	*temp;

	if (!redirections)
		return ;
	current = redirections;
	while (current)
	{
		temp = current->next;
		free(current->file);
		free(current->heredoc_content);
		free(current);
		current = temp;
	}
	return ;
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
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
}