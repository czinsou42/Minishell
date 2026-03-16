/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+  oui
	+#+           */
/*   Created: 2025/10/27 14:05:16 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/10 11:53:29 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redir_error(char *file)
{
	perror("minishell: ");
	perror(file);
	perror(": No such file or directory\n");
	exit(1);
}

int	apply_in(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_in;

	(void)cleanup;
	fd_in = open(redir->file, O_RDONLY);
	if (fd_in == -1)
	{
		print_redir_error(redir->file);
		return (1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (1);
	}
	close(fd_in);
	return (0);
}

int	apply_out(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_out;

	(void)cleanup;
	fd_out = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd_out == -1)
	{
		print_redir_error(redir->file);
		return (1);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (1);
	}
	close(fd_out);
	return (0);
}

int	apply_append(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_apnd;

	(void)cleanup;
	fd_apnd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd_apnd == -1)
	{
		print_redir_error(redir->file);
		return (1);
	}
	if (dup2(fd_apnd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (1);
	}
	close(fd_apnd);
	return (0);
}

int	apply_heredoc(t_redir *redir, t_cleanup *cleanup)
{
	int	pipe_fd[2];

	(void)cleanup;
	if (!redir->heredoc_content)
		return (1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (1);
	}
	if (write(pipe_fd[1], redir->heredoc_content,
			ft_strlen(redir->heredoc_content)) == -1)
	{
		perror("write");
		return (1);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (1);
	}
	close(pipe_fd[0]);
	free(redir->heredoc_content);
	redir->heredoc_content = NULL;
	return (0);
}
