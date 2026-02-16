/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/16 16:03:50 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_token(t_token *current)
{
	if (current->type == TOKEN_LPAREN || current->type == TOKEN_RPAREN)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `('\n", 2);
		return (1);
	}
	return (0);
}

static t_token	*skip_redirection(t_token *current)
{
	if (current->type == TOKEN_REDIR_APPEND
		|| current->type == TOKEN_REDIR_HEREDOC
		|| current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT)
	{
		current = current->next;
		if (current)
			current = current->next;
	}
	return (current);
}

static int	add_word(char **argv, int *i, t_token *current)
{
	if (current->type == TOKEN_WORD)
	{
		argv[*i] = remove_quote(current->str);
		if (!argv[*i])
			return (1);
		(*i)++;
	}
	return (0);
}

static int	fill_argv(char **argv, t_token *tokens, int length)
{
	int		i;
	t_token	*current;

	i = 0;
	current = tokens;
	while (current && i < length)
	{
		if (is_invalid_token(current))
			return (-1);
		current = skip_redirection(current);
		if (!current)
			break ;
		if (add_word(argv, &i, current))
			return (-1);
		current = current->next;
	}
	argv[i] = NULL;
	return (0);
}

char	**create_argv(t_token *tokens, int count, int length)
{
	char	**argv;

	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	if (fill_argv(argv, tokens, length) == -1)
	{
		ft_free_split(argv);
		return (NULL);
	}
	return (argv);
}
