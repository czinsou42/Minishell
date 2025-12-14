/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 07:44:19 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/14 12:09:09 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_var(char ***export_vars, char *name)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	if (!export_vars || !*export_vars || !name)
		return ;
	len = ft_strlen(name);
	while ((*export_vars)[i])
	{
		if (ft_strncmp((*export_vars)[i], name, len) == 0
			&& ((*export_vars)[i][len] == '='
				|| (*export_vars)[i][len] == '\0'))
		{
			free((*export_vars)[i]);
			j = i;
			while ((*export_vars)[j])
			{
				(*export_vars)[j] = (*export_vars)[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

int	builtin_unset(t_command *cmd, char ***env, char ***export_vars)
{
	int	i;

	if (!cmd || !cmd->argv)
		return (1);
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_env(cmd->argv[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n",
				cmd->argv[i]);
		}
		else
		{
			remove_var(env, cmd->argv[i]);
			remove_var(export_vars, cmd->argv[i]);
		}
		i++;
	}
	return (0);
}

