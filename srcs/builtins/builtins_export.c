/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:23:33 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/12 16:41:17 by czinsou          ###   ########.fr       */
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

static int	update_env(char ***envp, char *name, char *value)
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

int	builtin_export(t_command *cmd, char ***envp)
{
	int		i;
	char	*arg;
	char	*eq;
	char	*name;
	char	*value;

	if (!cmd || !cmd->argv || !envp)
		return (1);
	i = 1;
	while (cmd->argv[i])
	{
		arg = cmd->argv[i];
		eq = ft_strchr(arg, '=');
		if (!is_valid_env_name(arg))
		{
			printf("minishell: export: `%s': not a valid identifier\n", arg);
			i++;
			continue ;
		}
		if (eq)
		{
			name = ft_strndup(arg, eq - arg);
			value = eq + 1;
			if (update_env(envp, name, value))
				return (free(name), 1);
			free(name);
		}
		else
		{
			if (update_env(envp, arg, ""))
				return (1);
		}
		i++;
	}
	return (0);
}
