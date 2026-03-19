/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:35:07 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/19 15:18:03 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*token_last(t_token *tokens)
{
	while (tokens && tokens->next)
		tokens = tokens->next;
	return (tokens);
}

static int	handle_next_token(char *line, int *i, t_token **tokens,
		int *token_count)
{
	int	no_space;

	no_space = !ft_isspace(line[*i]);
	skip_spaces(line, i);
	if (line[*i] == '\0')
		return (0);
	if (line[*i] == '"' || line[*i] == '\'')
	{
		if (handle_quotes(line, i, tokens) == -1)
			return (-1);
	}
	else if (ft_isdelimiter(line[*i]))
	{
		if (handle_operator(line, i, tokens) == -1)
			return (-1);
	}
	else if (handle_word(line, i, tokens) == -1)
		return (-1);
	if (no_space && *token_count > 0 && token_last(*tokens)->type == TOKEN_WORD)
		token_last(*tokens)->joined = 1;
	(*token_count)++;
	return (1);
}

t_token	*lexer(char *line)
{
	t_token	*tokens;
	int		i;
	int		token_count;
	int		res;

	tokens = NULL;
	i = 0;
	token_count = 0;
	while (line[i])
	{
		res = handle_next_token(line, &i, &tokens, &token_count);
		if (res == -1)
			return (free_tokens(tokens), NULL);
		if (res == 0)
			break ;
	}
	return (tokens);
}
