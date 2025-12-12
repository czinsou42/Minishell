/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:35:07 by amwahab           #+#    #+#             */
/*   Updated: 2025/10/22 10:52:15 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(char *line)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_spaces(line, &i);
		if (line[i] == '\0')
			break ;
		if (line[i] == '"' || line [i] == '\'')
		{
			if (handle_quotes(line, &i, &tokens) == -1)
				return (free_tokens(tokens), NULL);
		}
		else if (ft_isdelimiter(line[i]))
		{
			if (handle_operator(line, &i, &tokens) == -1)
				return (free_tokens(tokens), NULL);
		}
		else if (handle_word(line, &i, &tokens) == -1)
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
