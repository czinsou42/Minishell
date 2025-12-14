/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_handle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 05:09:01 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/14 12:11:15 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_export_var(char ***export_vars, char *new_var, int size)
{
	char	**tmp;
	int		i;

	tmp = malloc(sizeof(char *) * (size + 2));
	if (!tmp)
		return (free(new_var), 1);
	i = 0;
	while (i < size)
	{
		tmp[i] = (*export_vars)[i];
		i++;
	}
	tmp[i] = new_var;
	tmp[i + 1] = NULL;
	free(*export_vars);
	*export_vars = tmp;
	return (0);
}

static char	*create_export_value(char *name, char *value)
{
	char	*new_var;

	if (value)
		new_var = ft_strjoin3(name, "=", value);
	else
		new_var = ft_strdup(name);
	if (!new_var)
		return (NULL);
	return (new_var);
}


static int	update_export_list(char ***export_vars, char *name, char *value)
{
	int		i;
	int		len;
	char	*new_var;

	len = ft_strlen(name);
	i = 0;
	while ((*export_vars)[i])
	{
		if (ft_strncmp((*export_vars)[i], name, len) == 0
			&& ((*export_vars)[i][len] == '='
				|| (*export_vars)[i][len] == '\0'))
		{
			free((*export_vars)[i]);
			new_var = create_export_value(name, value);
			if (!new_var)
				return (1);
			(*export_vars)[i] = new_var;
			return (0);
		}
		i++;
	}
	new_var = create_export_value(name, value);
	if (!new_var)
		return (1);
	return (create_export_var(export_vars, new_var, i));
}

int	handle_export_arg(char *arg, char ***env, char ***export_vars)
{
	char	*eq;
	char	*name;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (!is_valid_env(arg))
	{
		printf("minishell: export: `%s': not a valid identifier\n", arg);
		return (0);
	}
	if (eq)
	{
		name = ft_strndup(arg, eq - arg);
		value = eq + 1;
		update_env(env, name, value);
		update_export_list(export_vars, name, value);
		free(name);
	}
	else
	{
		update_export_list(export_vars, arg, NULL);
	}
	return (0);
}
