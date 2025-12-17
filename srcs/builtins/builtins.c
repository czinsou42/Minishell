/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:19:13 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/17 15:52:46 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_parent_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin_simple(t_command *cmd, char ***envp)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(cmd));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(cmd, *envp));
	return (1);
}

int	execute_builtin_parent(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd, envp));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd, envp, NULL));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd, envp, NULL));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd, cleanup));
	return (-1);
}