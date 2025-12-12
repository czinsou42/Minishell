/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:23:33 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/12 16:28:40 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_env(const char *name)
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

static void	create_env(char ***envp, char *name, char value)
{
	new_var = ft_strjoin3(name, "=", value);
	if (!new_var)
		return (1);
	tmp = malloc((i + 2)* sizeof(char *));
	if (!tmp)
		return (1);
	j = 0;
	while (j < i)
	{
		tmp[j] = (*envp)[j];
		j++;
	}
	tmp[i] = new_var;
	tmp[i + 1] = NULL;
	free(*envp);
	*envp = tmp;
}

static int	update_env(char ***envp, char *name, char *value)
{
	int		i;
	int		j;
	int		len;
	char	*new_var;
	char	**tmp;

	len = ft_strlen(name);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], name, len) && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			new_var = ft_strjoin3(name, "=", value);
			if (!new_var)
				return (1);
			(*envp)[i] = new_var;
			return (0);
		}
		i++;
	}
	new_var = ft_strjoin3(name, "=", value);
	if (!new_var)
		return (1);
	tmp = malloc((i + 2)* sizeof(char *));
	if (!tmp)
		return (1);
	j = 0;
	while (j < i)
	{
		tmp[j] = (*envp)[j];
		j++;
	}
	tmp[i] = new_var;
	tmp[i + 1] = NULL;
	free(*envp);
	*envp = tmp;
	return (0);
}