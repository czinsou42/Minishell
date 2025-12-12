/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:40:34 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/01 14:42:49 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_command(t_command *cmd, char **envp, t_cleanup *cleanup)
{
	int		status;
	pid_t	pid;
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
		return (execute_builtin_parent(cmd, &envp, cleanup));
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		apply_redirections(cmd->redirections);
		if (execute_builtin_simple(cmd) == 0)
			exit(0);
		path = get_path(cmd->argv[0], envp);
		execve(path, cmd->argv, envp);
		print_command_error(cmd->argv[0], 127);
		exit(127);
	}
	waitpid(pid, &status, 0);
	return (get_exit_code(status));
}
