/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:23:33 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/22 17:13:16 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_or_add_env(char ***envp, char *name, char *value)
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
	return (env_add(envp, name, value));
}

static void	print_export_line(char *var)
{
	char	*eq;
	char	*name;

	if (!var || !var)
		return ;
	eq = ft_strchr(var, '=');
	if (eq)
	{
		name = ft_strndup(var, eq - var);
		if (!name)
			return ;
		printf("declare -x %s=\"%s\"\n", name, eq + 1);
		free(name);
	}
	else
		printf("declare -x %s\n", var);
}

static void	print_all_exports(char **envp)
{
	int		i;
	char	**sorted;
	int		count;

	if (!envp)
		return ;
	count = 0;
	while (envp[count])
		count++;
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return ;
	i = 0;
	while (i < count)
	{
		sorted[i] = envp[i];
		i++;
	}
	sorted[count] = NULL;
	sort_env(sorted);
	i = 0;
	while (sorted[i])
	{
		print_export_line(sorted[i]);
		i++;
	}
	free(sorted);
}

int	handle_export_arg(char *arg, char ***env)
{
	char	*eq;
	char	*name;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		name = ft_strndup(arg, eq - arg);
		if (!name)
			return (1);
		if (!is_valid_env(name))
			return (printf("minishell: export: `%s': not a valid identifier\n",
					name), free(name), 0);
		value = eq + 1;
		update_or_add_env(env, name, value);
		for (int k = 0; (*env)[k]; k++)
			printf("DEBUG env[%d] = %s\n", k, (*env)[k]);
		free(name);
	}
	else
	{
		if (!is_valid_env(arg))
			return (printf("minishell: export: `%s': not a valid identifier\n",
					arg), 0);
	}
	return (0);
}

int	builtin_export(t_command *cmd, char ***envp)
{
	int	i;

	if (!cmd || !cmd->argv || !envp)
		return (1);
	if (!cmd->argv[1])
	{
		print_all_exports(*envp);
		return (0);
	}
	i = 1;
	while (cmd->argv[i])
	{
		handle_export_arg(cmd->argv[i], envp);
		i++;
	}
	return (0);
}
