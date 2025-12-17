/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 11:24:28 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/17 15:26:14 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[PATH_MAX];

	(void)cmd;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_env(t_command *cmd, char **envp)
{
	int	i;

	(void)cmd;
	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}


