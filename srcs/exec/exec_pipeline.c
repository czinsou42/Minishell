/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:57:07 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/22 18:54:39 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipeline_cmd(t_command *cmd, t_cleanup *cleanup,
			 char ***envp)
{
	char	*path;

	apply_redirections(cmd->redirections);
	if (execute_builtin_simple(cmd, envp) == 0)
		cleanup_and_exit(cleanup, 0);
	path = get_path(cmd->argv[0], *envp);
	if (!path)
	{
		print_command_error(cmd->argv[0], 127);
		cleanup_and_exit (cleanup, 127);
	}
	execve(path, cmd->argv, *envp);
	print_command_error(cmd->argv[0], 126);
	cleanup_and_exit(cleanup, 126);
	free_pipeline(cleanup->pipeline);
}

static pid_t	fork_pipeline_child(t_cleanup *cleanup, int prev_fd,
		int *pipefd, char ***envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		setup_child_pipe(prev_fd, pipefd, cleanup->pipeline->next != NULL);
		execute_pipeline_cmd(cleanup->pipeline->cmd, cleanup, envp);
	}
	if (cleanup->pipeline->next)
		close(pipefd[1]);
	if (prev_fd != -1)
		close(prev_fd);
	return (pid);
}

static void	run_pipeline_loop(t_cleanup *cleanup, int *prev_fd,
		char ***envp)
{
	int		pipefd[2];
	pid_t	pid;

	*prev_fd = -1;
	while (cleanup->pipeline)
	{
		if (cleanup->pipeline->next && pipe(pipefd) == -1)
			(perror("pipe"), cleanup_and_exit(cleanup, 1));
		pid = fork_pipeline_child(cleanup, *prev_fd, pipefd, envp);
		if (pid == -1)
			cleanup_and_exit(cleanup, 1);
		if (cleanup->pipeline->next)
			*prev_fd = pipefd[0];
		else
			*prev_fd = -1;
		cleanup->pipeline = cleanup->pipeline->next;
	}
	wait_all(pid);
}

int	exec_pipeline(t_node *node, char ***envp, t_cleanup *cleanup)
{
	int	prev_fd;

	setup_signals();
	cleanup->pipeline = extract_cmd(node);
	cleanup->head_pipeline = cleanup->pipeline;
	run_pipeline_loop(cleanup, &prev_fd, envp);
	if (prev_fd != -1)
		close(prev_fd);
	free_pipeline(cleanup->head_pipeline);
	cleanup->pipeline = NULL;
	return (g_exit_status);
}

// int exec_pipeline(t_node *node, char ***envp, t_cleanup *cleanup)
// {
//     int         prev_fd;
//     int         pipefd[2];
//     pid_t       pid;

//     setup_signals();
//     cleanup->pipeline = extract_cmd(node);
//     cleanup->head_pipeline = cleanup->pipeline;

//     prev_fd = -1;
//     while (cleanup->pipeline)
//     {
//         if (cleanup->pipeline->next && pipe(pipefd) == -1)
// 		{
// 			perror("pipe");
//             cleanup_and_exit(cleanup, 1);
// 		}

//         pid = fork_pipeline_child(cleanup, prev_fd, pipefd, envp);
//         if (pid == -1)
//             cleanup_and_exit(cleanup, 1);
//         if (cleanup->pipeline->next)
//             prev_fd = pipefd[0];
//         else
//             prev_fd = -1;

//         cleanup->pipeline = cleanup->pipeline->next;
//     }

//     if (prev_fd != -1)
//         close(prev_fd);

//     wait_all(pid);
// 	free_pipeline(cleanup->head_pipeline);
// 	cleanup->pipeline = NULL;
//     return (g_exit_status);
// }