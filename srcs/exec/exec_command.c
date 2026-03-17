/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/16 15:48:30 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd->redirections, cleanup) == 1)
	{
		cleanup_and_exit(cleanup, 1);
		return ;
	}
	path = get_path(cmd->argv[0], *envp);
	if (!path)
	{
		print_command_error(cmd->argv[0], 127);
		cleanup_and_exit(cleanup, 127);
		return ;
	}
	execve(path, cmd->argv, *envp);
	print_command_error(cmd->argv[0], 126);
	cleanup_and_exit(cleanup, 126);
}

static void	setup_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	restore_parent_signals(void)
{
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
}

static int	fork_and_wait(t_command *cmd, char ***envp,
		t_cleanup *cleanup)
{
	pid_t	pid;
	int		status;

	setup_parent_signals();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_child(cmd, envp, cleanup);
	waitpid(pid, &status, 0);
	restore_parent_signals();
	return (get_exit_code(status));
}

int	exec_command(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	int	saved_stdout;
	int	ret;

	if (!cmd || !cmd->argv || !cmd->argv[0] || !*cmd->argv[0])
		return (0);
	while (cmd->argv[0] && !*cmd->argv[0])
		cmd->argv++;
	if (!cmd->argv[0])
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
	{
		if (apply_redirections(cmd->redirections, cleanup) == 1)
		{
			g_exit_status = 1;
			return (1);
		}
		return (execute_builtin_parent(cmd, envp, cleanup));
	}
	if (is_simple_builtin(cmd->argv[0]))
	{
		saved_stdout = dup(STDOUT_FILENO);
		if (apply_redirections(cmd->redirections, cleanup) == 1)
		{
//			printf("frdggdf\n");
			g_exit_status = 1;
			return (1);
		}
		ret = execute_builtin_simple(cmd, envp);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		return (ret);
	}
	return (fork_and_wait(cmd, envp, cleanup));
}
