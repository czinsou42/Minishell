/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:34:05 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/17 15:17:02 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin3(char *a, char *b, char *c)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(a, b);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, c);
	free(tmp);
	return (res);
}

char	*ft_strndup(const char *s, int maxlen)
{
	int		i;
	char	*dst;

	dst = (char *)malloc(sizeof(char) * (maxlen + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (s[i] && i < maxlen)
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	is_valid_env(const char *name)
{
	int	i;

	if (!name)
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*temp;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strncmp(env[i], env[j], ft_strlen(env[i]) + 1) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}
