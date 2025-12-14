/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:11:15 by amwahab           #+#    #+#             */
/*   Updated: 2025/12/14 11:12:33 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_command_error(char *cmd, int error_type)
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
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (-1);
}

void	cleanup_and_exit(t_cleanup *cleanup, int status)
{
	free(cleanup->line);
	free_tokens(cleanup->tokens);
	free_ast(cleanup->ast);
	exit(status);
}
