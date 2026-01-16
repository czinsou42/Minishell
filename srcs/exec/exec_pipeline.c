/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:57:07 by amwahab           #+#    #+#             */
/*   Updated: 2026/01/16 15:56:43 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	exec_pipeline(t_node *node, char ***envp, t_cleanup *cleanup)
// {
// 	t_pipeline	*pipeline;

// 	// 1. extract cmd from node if node->left-type == CMD
// 	pipeline = extract_cmd(node);
// 	// 2.
// }


static void	handle_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			g_exit_status = 130;
		else if (sig == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			g_exit_status = 131;
		}
	}
}

void	wait_all(pid_t	g_signal)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0)
			break ;
		if (pid == g_signal)
			handle_status(status);
	}
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
}

int	exec_pipeline(t_node *node, char ***envp, t_cleanup *cleanup)
{
	t_pipeline	*pipeline;
	int			prev_fd;
	int			pipefd[2];
	pid_t		pid;
	char		*path;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pipeline = extract_cmd(node);
	prev_fd = -1;
	(void)cleanup;
	while (pipeline)
	{
		if (pipeline->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		pid = fork();
		if (pid == -1)
			return (perror("fork"),1);
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_IGN);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (pipeline->next)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			apply_redirections(pipeline->cmd->redirections);
			if (execute_builtin_simple(pipeline->cmd, envp) == 0)
				exit(0);
			path = get_path(pipeline->cmd->argv[0], *envp);
			if (!path)
			{
				print_command_error(pipeline->cmd->argv[0], 127);
				exit(127);
			}
			execve(path, pipeline->cmd->argv, *envp);
			print_command_error(pipeline->cmd->argv[0], 126);
			exit(126);
		}
		if (pipeline->next)
			close(pipefd[1]);
		if (prev_fd != -1)
			close(prev_fd);
		prev_fd = pipeline->next ? pipefd[0] : -1;
		pipeline = pipeline->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_all(pid);
	return (g_exit_status);
}
