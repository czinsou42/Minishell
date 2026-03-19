/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/19 13:43:46 by lebertau         ###   ########.fr       */
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

static void	skip_empty_args(char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i] && !*argv[i])
	{
		free(argv[i]);
		i++;
	}
	if (i > 0)
	{
		j = 0;
		while (argv[i + j])
		{
			argv[j] = argv[i + j];
			j++;
		}
		argv[j] = NULL;
	}
}

int	exec_command(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	if (!cmd || !cmd->argv)
		return (0);
	skip_empty_args(cmd->argv);
	// Exec commandes vides contenant juste des redirections (echo < fichier_non_existant)
	if (!cmd->argv[0])
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
