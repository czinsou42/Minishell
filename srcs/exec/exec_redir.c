/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:58:04 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/16 15:39:08 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_redir *redir)
{
	t_redir	*current;

	current = redir;
	while (current)
	{
		if (current->type == REDIR_IN)
			apply_in(current);
		else if (current->type == REDIR_OUT)
			apply_out(current);
		else if (current->type == REDIR_APPEND)
			apply_append(current);
		else if (current->type == REDIR_HEREDOC)
			apply_heredoc(current);
		current = current->next;
	}
}
