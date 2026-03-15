/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:51:41 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/15 12:28:37 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(char *line, int *i, t_token **tokens)
{
	t_token_type	type;
	t_token			*token;
	int				len;

	type = get_operator_type(&line[*i]);
	if (type == TOKEN_ERROR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(line[*i], 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC
		|| type == TOKEN_OR || type == TOKEN_AND)
		len = 2;
	else
		len = 1;
	token = create_token(type, NO_QUOTE, &line[*i], len);
	if (!token)
		return (-1);
	token_add_back(tokens, token);
	(*i) += len;
	return (0);
}

int	handle_word(char *line, int *i, t_token **tokens)
{
	int		start;
	t_token	*token;
	int		len;

	start = *i;
	while (line[*i] && !ft_isspace(line[*i]) && !ft_isdelimiter(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			char quote = line[*i];
			(*i)++;
			while (line[*i] && line[*i] != quote)
				(*i)++;
			if (line[*i] == '\0')
				return (-1);
			(*i)++;
		}
		else
			(*i)++;
	}
	len = *i - start;
	token = create_token(TOKEN_WORD, get_quote_type(line, &start), &line[start], len);
	if (!token)
		return (-1);
	token_add_back(tokens, token);
	return (0);
}
/*
int	handle_quotes(char *line, int *i, t_token **tokens)
{
	t_quote_type	quote_type;
	t_token			*token;
	char			quote_char;
	int				j;
	int				len;

	quote_type = get_quote_type(line, i);
	j = (*i);
	(*i)++;
	while (line[*i] != quote_char && line[*i] != '\0')
		(*i)++;
	if (line[*i] == '\0')
		return (-1);
	len = *i - j + 1;
	token = create_token(TOKEN_WORD, quote_type, &line[j], len);
	if (!token)
		return (-1);
	token_add_back(tokens, token);
	(*i)++;
	return (0);
}
*/