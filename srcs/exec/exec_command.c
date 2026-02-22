/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/22 18:25:43 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

