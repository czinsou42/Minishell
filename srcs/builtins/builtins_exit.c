/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 10:54:13 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/21 13:53:46 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
}

int	get_exit_value(const char *str)
{
	long	val;

	val = ft_atol(str);
	return ((unsigned char)val);
}

int	return_exit_error(char *arg, int error_type)
{
	if (error_type == 1)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else if (error_type == 2)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		if (arg)
			ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
	return (2);
}

int	builtin_exit(t_command *cmd, t_cleanup *cleanup)
{
	int	exit_code;

	printf("exit\n");
	rl_clear_history();
	if (!cmd->argv[1])
	{
		close_extra_fds();
		cleanup_and_exit(cleanup, g_exit_status);
	}
	if (!is_numeric_exit(cmd->argv[1]))
	{
		return_exit_error(cmd->argv[1], 2);
		close_extra_fds();
		cleanup_and_exit(cleanup, 2);
	}
	if (cmd->argv[2])
		return (return_exit_error(NULL, 1));
	exit_code = get_exit_value(cmd->argv[1]);
	g_exit_status = exit_code;
	close_extra_fds();
	cleanup_and_exit(cleanup, exit_code);
	return (g_exit_status);
}
