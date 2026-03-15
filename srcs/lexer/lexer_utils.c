/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:16:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/15 12:23:18 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char *line, int *i)
{
	while (ft_isspace(line[*i]))
		(*i)++;
}

t_token_type	get_operator_type(char *str)
{
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_REDIR_APPEND);
	if (str[0] == '<' && str[1] == '<')
		return (TOKEN_REDIR_HEREDOC);
	if (str[0] == '|' && str[1] == '|')
		return (TOKEN_OR);
	if (str[0] == '&' && str[1] == '&')
		return (TOKEN_AND);
	if (str[0] == '|')
		return (TOKEN_PIPE);
	if (str[0] == '<')
		return (TOKEN_REDIR_IN);
	if (str[0] == '>')
		return (TOKEN_REDIR_OUT);
	if (str[0] == '(')
		return (TOKEN_LPAREN);
	if (str[0] == ')')
		return (TOKEN_RPAREN);
	return (TOKEN_ERROR);
}

t_quote_type	get_quote_type(char *line, int *i)
{
	t_quote_type	quote_type;

	//printf("line[%d] = '%c'\n", *i, line[*i]);
	if (line[*i] == '"')
		quote_type = DOUBLE_QUOTE;
	else if (line[*i] == '\'')
		quote_type = SINGLE_QUOTE;
	else
		return (NO_QUOTE);
	return (quote_type);
}
