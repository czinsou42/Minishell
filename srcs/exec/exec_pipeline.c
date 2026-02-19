/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:57:07 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/19 13:19:27 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static void	setup_child_pipe(int prev_fd, int *pipefd, int has_next)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

static void	execute_pipeline_cmd(t_command *cmd, char ***envp)
{
	char	*path;

	apply_redirections(cmd->redirections);
	if (execute_builtin_simple(cmd, envp) == 0)
		exit(0);
	path = get_path(cmd->argv[0], *envp);
	if (!path)
	{
		print_command_error(cmd->argv[0], 127);
		exit(127);
	}
	execve(path, cmd->argv, *envp);
	print_command_error(cmd->argv[0], 126);
	exit(126);
}

static pid_t	fork_pipeline_child(t_pipeline *pipeline, int prev_fd,
		int *pipefd, char ***envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		setup_child_pipe(prev_fd, pipefd, pipeline->next != NULL);
		execute_pipeline_cmd(pipeline->cmd, envp);
	}
	if (pipeline->next)
		close(pipefd[1]);
	if (prev_fd != -1)
		close(prev_fd);
	return (pid);
}

int	exec_pipeline(t_node *node, char ***envp, t_cleanup *cleanup)
{
	t_pipeline	*pipeline;
	int			prev_fd;
	int			pipefd[2];
	pid_t		pid;

	setup_signals();
	pipeline = extract_cmd(node);
	prev_fd = -1;
	(void)cleanup;
	while (pipeline)
	{
		if (pipeline->next && pipe(pipefd) == -1)
			return (perror("pipe"), 1);
		pid = fork_pipeline_child(pipeline, prev_fd, pipefd, envp);
		if (pid == -1)
			return (1);
		if (pipeline->next)
			prev_fd = pipefd[0];
		else
			prev_fd = -1;
		pipeline = pipeline->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (wait_all(pid), g_exit_status);
}
