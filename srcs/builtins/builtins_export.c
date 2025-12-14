/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:23:33 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/14 12:08:09 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_env(char ***envp, char *name, char *value, int i)
{
	int		j;
	char	*new_var;
	char	**tmp;

	j = 0;
	new_var = ft_strjoin3(name, "=", value);
	if (!new_var)
		return (1);
	tmp = malloc(sizeof(char *) * (i + 2));
	if (!tmp)
		return (free(new_var), 1);
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

int	update_env(char ***envp, char *name, char *value)
{
	int		i;
	int		len;
	char	*new_var;

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
	return (create_env(envp, name, value, i));
}

void	print_export_vars(char **export_vars)
{
	int		i;

	sort_env(export_vars);
	i = 0;
	while (export_vars[i])
	{
		printf("declare -x %s\n", export_vars[i]);
		i++;
	}
}

int	builtin_export(t_command *cmd, char ***env, char ***export_vars)
{
	int	i;

	if (!cmd || !cmd->argv)
		return (1);
	if (!cmd->argv[1])
	{
		print_export_vars(*export_vars);
		return (0);
	}
	i = 1;
	while (cmd->argv[i])
	{
		handle_export_arg(cmd->argv[i], env, export_vars);
		i++;
	}
	return (0);
}
