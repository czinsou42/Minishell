/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:58:04 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/15 17:13:36 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirections(t_redir *redir, t_cleanup *cleanup)
{
	t_redir	*current;

	(void)cleanup;
	current = redir;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			if (apply_in(current, cleanup))
				return (1);
		}
		else if (current->type == REDIR_OUT)
		{
			if (apply_out(current, cleanup))
				return (1);
		}
		else if (current->type == REDIR_APPEND)
		{
			if (apply_append(current, cleanup))
				return (1);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			if (apply_heredoc(current, cleanup))
				return (1);
		}
		current = current->next;
	}
	return (0);
}
