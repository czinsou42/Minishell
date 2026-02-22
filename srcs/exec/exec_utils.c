/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:11:15 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/22 18:53:17 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command_error(char *cmd, int error_type)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (error_type == 127)
		ft_putstr_fd("command not found\n", 2);
	else if (error_type == 126)
		ft_putstr_fd("Permission denied\n", 2);
}

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (g_exit_status = WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (g_exit_status = (128 + WTERMSIG(status)));
	return (-1);
}

void	cleanup_and_exit(t_cleanup *cleanup, int status)
{
	t_pipeline	*tmp;
	
	if (cleanup->line)
		free(cleanup->line);
	if (cleanup->envp)
		free_envp(cleanup->envp);
	if (cleanup->head_pipeline)
    {
		while (cleanup->head_pipeline)
        {
			tmp = cleanup->head_pipeline->next;
			free(cleanup->head_pipeline);
			cleanup->head_pipeline = tmp;
		}
		cleanup->pipeline = NULL;
		cleanup->head_pipeline = NULL;
	}
	if (cleanup->tokens)
		free_tokens(cleanup->tokens);
	if (cleanup->ast)
		free_ast(cleanup->ast);
	exit(status);
}

void	setup_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_pipe(int prev_fd, int *pipefd, int has_next)
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
