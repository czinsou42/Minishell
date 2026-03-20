/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebertau <lebertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by amwahab           #+#    #+#             */
/*   Updated: 2026/03/20 16:07:56 by lebertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_add_back(t_redir **lst, t_redir *new)
{
	t_redir	*current;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

t_node	*create_command_node(t_token *tokens, int length)
{
	t_command	*command;
	t_node		*node;

	command = parse_command(tokens, length);
	if (!command)
		return (NULL);
	node = create_node(NODE_COMMAND, command);
	return (node);
}

char	*remove_quote(char *str)
{
	char	*res;
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	res = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'') || (str[i] == '"'))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				res[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			res[j++] = str[i++];
	}
	return ((res[j] = '\0'), res);
}

int	print_parser_redir_error(t_token *token)
{
	if (token->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell :syntax error near unexpected token `", 2);
		ft_putstr_fd(token->str, 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	return (0);
}

void	print_unexpected_token(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
		2);
}
