/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:58:04 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/10 18:35:37 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_redir *redir)
{
	t_redir	*current;

	printf("apply redirection");
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
