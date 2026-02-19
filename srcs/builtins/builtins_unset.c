/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 07:44:19 by czinsou           #+#    #+#             */
/*   Updated: 2026/02/19 13:18:39 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_var(char ***envp, char *name)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	if (!envp || !*envp || !name)
		return ;
	len = ft_strlen(name);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, len) == 0
			&& ((*envp)[i][len] == '='
				|| (*envp)[i][len] == '\0'))
		{
			free((*envp)[i]);
			j = i;
			while ((*envp)[j])
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

int	builtin_unset(t_command *cmd, char ***env)
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
			remove_var(env, cmd->argv[i]);
		i++;
	}
	return (0);
}
