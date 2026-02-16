/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/16 15:19:55 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	exec_command(t_command *cmd, char **envp, t_cleanup *cleanup)
// {
// 	pid_t	pid;
// 	int		status;
// 	char	*path;

// 	if (is_parent_builtin(cmd->argv[0]))
// 		return (execute_builtin_parent(cmd, &envp, cleanup));
// 	pid = fork();
// 	if (pid == -1)
// 		return (-1);
// 	if (pid == 0)
// 	{
// 		path = get_path(cmd->argv[0], envp);
// 		if (!path)
// 		{
// 			print_command_error(cmd->argv[0], 127);
// 			free_ast(cleanup->ast);
// 			free_tokens(cleanup->tokens);
// 			free(cleanup->line);
// 			exit(127);
// 		}
// 		apply_redirections(cmd->redirections);
// 		execve(path, cmd->argv, envp);
// 		print_command_error(cmd->argv[0], 126);
// 		free(path);
// 		free_ast(cleanup->ast);
// 		free_tokens(cleanup->tokens);
// 		free(cleanup->line);
// 		exit(126);
// 	}
// 	waitpid(pid, &status, 0);
// 	return(get_exit_code(status));
// }

int	exec_command(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	pid_t	pid;
	int		status;
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
		return (execute_builtin_parent(cmd, envp, cleanup));
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		apply_redirections(cmd->redirections);
		if (execute_builtin_simple(cmd, envp) == 0)
			exit(0);
		path = get_path(cmd->argv[0], *envp);
		if (!path)
			(print_command_error(cmd->argv[0], 127), exit(127));
		execve(path, cmd->argv, *envp);
		(print_command_error(cmd->argv[0], 126), exit(126));
	}
	waitpid(pid, &status, 0);
	return (get_exit_code(status));
}
