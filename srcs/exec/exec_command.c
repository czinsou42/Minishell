/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/19 09:07:39 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_command(t_command *cmd, char **envp, t_cleanup *cleanup)
{
	pid_t	pid;
	int		status;
	char	*path;
	
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		path = get_path(cmd->argv[0], envp);
		if (!path)
		{
			print_command_error(cmd->argv[0], 127);
			free_ast(cleanup->ast);
			free_tokens(cleanup->tokens);
			free(cleanup->line);
			exit(127);
		}
		apply_redirections(cmd->redirections);
		execve(path, cmd->argv, envp);
		print_command_error(cmd->argv[0], 126);
		free(path);
		free_ast(cleanup->ast);
		free_tokens(cleanup->tokens);
		free(cleanup->line);
		exit(126);
	}
	waitpid(pid, &status, 0);
	return(get_exit_code(status));
}
