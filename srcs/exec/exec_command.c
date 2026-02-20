/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/20 17:34:13 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	exec_command(t_command *cmd, char ***envp, t_cleanup *cleanup)
// {
// 	pid_t	pid;
// 	int		status;
// 	char	*path;

// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 		return (0);
// 	if (is_parent_builtin(cmd->argv[0]))
// 		return (execute_builtin_parent(cmd, envp, cleanup));
// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("fork"), 1);
// 	if (pid == 0)
// 	{
// 		apply_redirections(cmd->redirections);
// 		if (execute_builtin_simple(cmd, envp) == 0)
// 			exit(0);
// 		path = get_path(cmd->argv[0], *envp);
// 		if (!path)
// 			(print_command_error(cmd->argv[0], 127), exit(127));
// 		execve(path, cmd->argv, *envp);
// 		(print_command_error(cmd->argv[0], 126), exit(126));
// 	}
// 	waitpid(pid, &status, 0);
// 	return (get_exit_code(status));
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
	if (is_simple_builtin(cmd->argv[0]))
		return (execute_builtin_simple(cmd, envp));
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		apply_redirections(cmd->redirections);
		if (execute_builtin_parent(cmd, envp, cleanup) == 0)
			exit(0);
		path = get_path(cmd->argv[0], *envp);
		if (!path)
			(print_command_error(cmd->argv[0], 127), cleanup_and_exit(cleanup,
					127));
		execve(path, cmd->argv, *envp);
		(print_command_error(cmd->argv[0], 126), cleanup_and_exit(cleanup,
				126));
	}
	return (waitpid(pid, &status, 0), get_exit_code(status));
}

// int	execute_builtin_simple(t_command *cmd, char ***envp)
// {
// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 		return (1);
// 	if (ft_strcmp(cmd->argv[0], "echo") == 0)
// 		return (builtin_echo(cmd));
// 	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
// 		return (builtin_pwd(cmd));
// 	if (ft_strcmp(cmd->argv[0], "env") == 0)
// 		return (builtin_env(cmd, *envp));
// 	return (1);
// }

int	is_simple_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}
