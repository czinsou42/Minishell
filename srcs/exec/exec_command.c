/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/20 15:13:08 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_dir(char *path)
{
	struct stat	s;

	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
		return (1);
	return (0);
}

static void	exec_child(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	char	*path;

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
	path = get_path(cmd->argv[0], *envp);
	if (!path)
	{
		print_command_error(cmd->argv[0], 127);
		cleanup_and_exit(cleanup, 127);
	}
	if (ft_strchr(cmd->argv[0], '/'))
	{
		if (is_dir(cmd->argv[0]))
		{
			print_command_error(cmd->argv[0], 126);
			cleanup_and_exit(cleanup, 126);
		}
		else if (access(cmd->argv[0], X_OK) != 0)
		{
			print_command_error(cmd->argv[0], 126);
			cleanup_and_exit(cleanup, 126);
		}
		execve(cmd->argv[0], cmd->argv, *envp);
		print_command_error(cmd->argv[0], 126);
		cleanup_and_exit(cleanup, 126);
	}
	execve(path, cmd->argv, *envp);
	free(path);
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

static int	fork_and_wait(t_command *cmd, char ***envp, t_cleanup *cleanup)
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
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	if (!cmd || !cmd->argv)
		return (0);
	// Exec commandes vides contenant juste des redirections (echo < fichier_non_existant)
	if (!cmd->argv[0])
	{
		if (cmd->redirections)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			ret = apply_redirections(cmd->redirections, cleanup);
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			if (ret)
				g_exit_status = ret;
			return (ret);
		}
		return (0);
	}
	// Exec normal
	if (is_parent_builtin(cmd->argv[0]) || is_simple_builtin(cmd->argv[0]))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (apply_redirections(cmd->redirections, cleanup) == 1)
		{
			g_exit_status = 1;
			ret = 1;
		}
		else if (is_parent_builtin(cmd->argv[0]))
			ret = execute_builtin_parent(cmd, envp, cleanup);
		else
			ret = execute_builtin_simple(cmd, envp);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (ret);
	}
	return (fork_and_wait(cmd, envp, cleanup));
}
