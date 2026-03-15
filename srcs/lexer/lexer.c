/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:35:07 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/15 11:50:43 by lebertau         ###   ########.fr       */
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
		if (ft_isdelimiter(line[i]))
		{
			if (handle_operator(line, &i, &tokens) == -1)
				return (free_tokens(tokens), NULL);
		}
		else if (handle_word(line, &i, &tokens) == -1)
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
