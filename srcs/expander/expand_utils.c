/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:54:51 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/19 14:21:02 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var_len(char *str, int *i, char **envp)
{
	int		j;
	char	*tmp;
	char	*value;

	j = *i + 1;
	while (ft_isalnum(str[j]) || str[j] == '_')
		j++;
	tmp = ft_substr(str, *i + 1, j - *i - 1);
	if (!tmp)
		return (-1);
	value = ft_getenv(tmp, envp);
	free(tmp);
	*i = j - 1;
	if (value)
		return (ft_strlen(value));
	return (0);
}

int	expanded_len(char *str, char **envp)
{
	int	i;
	int	total_len;
	int	len;

	i = -1;
	total_len = 0;
	while (str[++i])
	{
		if (str[i] == '$' && is_valid_var_char(str[i + 1]))
		{
			len = get_var_len(str, &i, envp);
			if (len == -1)
				return (0);
			total_len += len;
		}
		else
			total_len++;
	}
	return (total_len);
}

int	copy_var_value(t_expand_tokens *tokens, int *i, int k)
{
	int		j;
	char	*tmp;
	char	*value;
	int		len;

	j = *i + 1;
	while (ft_isalnum(tokens->str[j]) || tokens->str[j] == '_')
		j++;
	tmp = ft_substr(tokens->str, *i + 1, j - *i - 1);
	if (!tmp)
		return (-1);
	value = ft_getenv(tmp, tokens->envp);
	free(tmp);
	*i = j - 1;
	if (!value)
		return (k);
	len = 0;
	while (value[len])
		tokens->result[k++] = value[len++];
	return (k);
}

char	*copy_expanded_str(t_expand_tokens *tokens)
{
	int	i;
	int	k;

	i = -1;
	k = 0;
	while (tokens->str[++i])
	{
		if (tokens->str[i] == '$' && is_valid_var_char(tokens->str[i + 1]))
		{
			k = copy_var_value(tokens, &i, k);
			if (k == -1)
				return (NULL);
		}
		else
			tokens->result[k++] = tokens->str[i];
	}
	tokens->result[k] = '\0';
	return (tokens->result);
}

int	is_valid_var_char(char c)
{
	if (ft_isalpha(c) || c == '_')
		return (1);
	return (0);
}
