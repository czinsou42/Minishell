/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:49:44 by amwahab           #+#    #+#             */
/*   Updated: 2025/11/19 09:02:06 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*token;
	t_node		*node;
	t_cleanup	cleanup;

	(void)argc;
	(void)argv;
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			clear_history();
			break ;
		}
		else if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		token = lexer(line);
		if (!token)
		{
			free(line);
			continue ;
		}
		if (process_heredoc(token) == -1)
		{
			free(line);
			free_tokens(token);
			continue ;
		}
		expander(token, envp);
		node = parse(token, ft_tokens_size(token));
		if (!node)
		{
			free(line);
			free_tokens(token);
			continue ;
		}
		cleanup.line = line;
		cleanup.ast = node;
		cleanup.tokens = token;
		if (exec_ast(node, envp, &cleanup) == -1)
		{
			free(line);
			free_tokens(token);
			free_ast(node);
			continue ;
		}
		free(line);
		free_tokens(token);
		free_ast(node);
	}
	return (0);
}
