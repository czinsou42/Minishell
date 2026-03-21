/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:11:15 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/21 14:57:33 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command_error(char *cmd, int error_type)
{
	struct stat	path_stat;

	if (!cmd)
		cmd = "minishell";
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (error_type == 127)
	{
		if (ft_strchr(cmd, '/'))
			ft_putstr_fd("No such file or directory\n", 2);
		else
			ft_putstr_fd("command not found\n", 2);
	}
	else if (error_type == 126)
	{
		if (stat(cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
			ft_putstr_fd("Is a directory\n", 2);
		else
			ft_putstr_fd("Permission denied\n", 2);
	}
}

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (g_exit_status = WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (g_exit_status = (128 + WTERMSIG(status)));
	return (-1);
}

void	setup_signals(void)
{
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	setup_child_pipe(int prev_fd, int *pipefd, int has_next)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next && pipefd)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}
