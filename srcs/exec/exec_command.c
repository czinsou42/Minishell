/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/21 14:57:33 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_binary(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	char		*path;
	struct stat	s;

	path = get_path(cmd->argv[0], *envp);
	if (!path)
	{
		print_command_error(cmd->argv[0], 127);
		cleanup_and_exit(cleanup, 127);
	}
	if (ft_strchr(cmd->argv[0], '/'))
	{
		if ((stat(cmd->argv[0], &s) == 0 && S_ISDIR(s.st_mode))
			|| access(cmd->argv[0], X_OK) != 0)
		{
			print_command_error(cmd->argv[0], 126);
			cleanup_and_exit(cleanup, 126);
		}
		execve(cmd->argv[0], cmd->argv, *envp);
	}
	execve(path, cmd->argv, *envp);
	free(path);
	print_command_error(cmd->argv[0], 126);
	cleanup_and_exit(cleanup, 126);
}

static void	exec_child(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd || !cmd->argv || !cmd->argv[0])
		cleanup_and_exit(cleanup, 0);
	if (apply_redirections(cmd->redirections, cleanup) == 1)
		cleanup_and_exit(cleanup, 1);
	if (ft_strcmp(cmd->argv[0], ".") == 0)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		cleanup_and_exit(cleanup, 2);
	}
	if (ft_strcmp(cmd->argv[0], "..") == 0)
	{
		print_command_error("..", 127);
		cleanup_and_exit(cleanup, 127);
	}
	exec_binary(cmd, envp, cleanup);
}

static int	exec_builtin_or_redir(t_command *cmd, char ***envp,
		t_cleanup *cleanup)
{
	int	in;
	int	out;
	int	ret;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	ret = apply_redirections(cmd->redirections, cleanup);
	if (ret)
		g_exit_status = ret;
	else if (cmd->argv[0])
	{
		if (is_parent_builtin(cmd->argv[0]))
			ret = execute_builtin_parent(cmd, envp, cleanup);
		else
			ret = execute_builtin_simple(cmd, envp);
	}
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
	return (ret);
}

static int	fork_and_wait(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_child(cmd, envp, cleanup);
	waitpid(pid, &status, 0);
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
	return (get_exit_code(status));
}

int	exec_command(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	if (!cmd || !cmd->argv)
		return (0);
	if (!cmd->argv[0] && !cmd->redirections)
		return (0);
	if (!cmd->argv[0] || is_parent_builtin(cmd->argv[0])
		|| is_simple_builtin(cmd->argv[0]))
		return (exec_builtin_or_redir(cmd, envp, cleanup));
	return (fork_and_wait(cmd, envp, cleanup));
}
