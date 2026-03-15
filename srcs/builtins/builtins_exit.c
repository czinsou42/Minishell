/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 10:54:13 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/15 12:45:07 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric_exit(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	get_exit_value(const char *str)
{
	long	val;

	val = ft_atol(str);
	return ((unsigned char)val);
}

int	builtin_exit(t_command *cmd, t_cleanup *cleanup)
{
	int	exit_code;

	printf("exit\n");
	rl_clear_history();
	rl_free_line_state();
	if (!cmd->argv[1])
	{
		cleanup_and_exit(cleanup, g_exit_status);
	}
	if (!is_numeric_exit(cmd->argv[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", cmd->argv[1]);
		cleanup_and_exit(cleanup, 2);
	}
	if (cmd->argv[2])
		return (printf("minishell: exit: too many arguments\n"), 1);
	exit_code = get_exit_value(cmd->argv[1]);
	g_exit_status = exit_code;
	cleanup_and_exit(cleanup, exit_code);
	return (g_exit_status);
}
