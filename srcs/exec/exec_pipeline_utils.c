/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:20:25 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/19 10:39:55 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pipeline	*new_pipeline(t_command *cmd)
{
	t_pipeline	*new;

	new = malloc(sizeof(t_pipeline));
	if (!new)
		return (NULL);
	new->cmd = cmd;
	new->next = NULL;
	return (new);
}

static void	pipeline_add_back(t_pipeline **lst, t_pipeline *new)
{
	t_pipeline	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_pipeline	*extract_cmd(t_node *node)
{
	t_pipeline	*list;
	t_pipeline	*left_list;
	t_pipeline	*right_list;

	if (!node)
		return (NULL);
	if (node->type == NODE_COMMAND)
		return (new_pipeline(node->command));
	if (node->type != NODE_PIPE)
		return (NULL);
	left_list = extract_cmd(node->left);
	right_list = extract_cmd(node->right);
	list = left_list;
	if (!list)
		list = right_list;
	else
		pipeline_add_back(&list, right_list);
	return (list);
}

void	wait_all(pid_t g_signal)
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

void	handle_status(int status)
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
