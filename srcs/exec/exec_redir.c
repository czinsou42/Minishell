/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:58:04 by czinsou           #+#    #+#             */
/*   Updated: 2026/03/21 14:57:33 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_redirections_get_return(t_redir *redir, t_cleanup *cleanup)
{
	if (redir->type == REDIR_IN)
		return (apply_in(redir, cleanup));
	else if (redir->type == REDIR_OUT)
		return (apply_out(redir, cleanup));
	else if (redir->type == REDIR_APPEND)
		return (apply_append(redir, cleanup));
	else if (redir->type == REDIR_HEREDOC)
		return (apply_heredoc(redir, cleanup));
	return (0);
}

int	apply_redirections(t_redir *redir, t_cleanup *cleanup)
{
	t_redir	*current;

	current = redir;
	while (current)
	{
		if (apply_redirections_get_return(current, cleanup) == 1)
			return (1);
		current = current->next;
	}
	return (0);
}
