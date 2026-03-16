/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 11:24:28 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/15 16:32:42 by czinsou          ###   ########.fr       */
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

static int	print_echo_arg(char *arg)
{
	if (write(STDOUT_FILENO, arg, ft_strlen(arg)) == -1)
		return (-1);
	return (0);
}

static int	is_nl(char *args)
{
	int	j;

	j = 1;
	if (args[0] == '-' && args[1])
	{
		while (args[j] && args[j] == 'n')
			j++;
		if (args[j] == 0)
			return (1);
	}
	return (0);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (cmd->argv[i] && is_nl(cmd->argv[i]) == 1)
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		if (print_echo_arg(cmd->argv[i]) == -1)
			return (1);
		if (cmd->argv[i + 1])
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (1);
		i++;
	}
	if (newline)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (1);
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
