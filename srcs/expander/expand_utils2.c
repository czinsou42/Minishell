/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 14:43:20 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/20 14:57:17 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *name, char **envp)
{
	int	i;
	int	len;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	is_valid_var_char(char c)
{
	if (ft_isalpha(c) || c == '_' || c == '?')
		return (1);
	return (0);
}

void	apply_clean(t_token *tokens)
{
	char	*clean_str;

	clean_str = remove_quote(tokens->str);
	if (clean_str)
	{
		free(tokens->str);
		tokens->str = clean_str;
	}
}

void	split_token_on_spaces(t_token *token)
{
	char	**parts;
	int		i;
	t_token	*new;
	t_token	*last;

	if (!token->str || !ft_strchr(token->str, ' '))
		return ;
	parts = ft_split(token->str, ' ');
	if (!parts || !parts[0])
		return (ft_free_split(parts));
	free(token->str);
	token->str = ft_strdup(parts[0]);
	last = token;
	i = 1;
	while (parts[i])
	{
		new = create_token(TOKEN_WORD, NO_QUOTE, parts[i], ft_strlen(parts[i]));
		if (!new)
			break ;
		new->next = last->next;
		last->next = new;
		last = new;
		i++;
	}
	ft_free_split(parts);
}
