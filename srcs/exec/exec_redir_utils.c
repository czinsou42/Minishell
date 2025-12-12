/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+  oui  +#+           */
/*   Created: 2025/10/27 14:05:16 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/10 11:53:29 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_redir_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
	exit(1);
}
void	apply_in(t_redir *redir)
{
	int	fd_in;
	
	fd_in = open(redir->file, O_RDONLY);
	if(fd_in == -1)
		print_redir_error(redir->file);
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(fd_in);
}

void	apply_out(t_redir *redir)
{
	int	fd_out;
	
	fd_out = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(fd_out == -1)
		print_redir_error(redir->file);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(fd_out);
}

void	apply_append(t_redir *redir)
{
	int	fd_apnd;
	
	fd_apnd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if(fd_apnd == -1)
		print_redir_error(redir->file);
	if (dup2(fd_apnd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(fd_apnd);
}

void	apply_heredoc(t_redir *redir)
{
	int	pipe_fd[2];

	if (!redir->heredoc_content)
		exit(1);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		exit(1);
	}
	if (write(pipe_fd[1], redir->heredoc_content, ft_strlen(redir->heredoc_content)) == -1)
	{
		perror("minishell: write");
		exit(1);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(pipe_fd[0]);
	printf("apply heredoc");
}
