/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:49:44 by amwahab           #+#    #+#             */
/*   Updated: 2025/12/17 17:56:04 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler_signal(int sig)
{
	(void)sig;
}
// Dans ton init_shell() ou main()
char	**copy_envp(char **envp)
{
	int		i;
	int		count;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]); // ← COPIE chaque string
		if (!new_envp[i])
			return (NULL); // TODO: free ce qui a été alloué
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*token;
	t_node		*node;
	t_cleanup	cleanup;
	char		**my_envp;

	(void)argc;
	(void)argv;
	my_envp = copy_envp(envp);
	if (!my_envp)
		return (1);
	signal(SIGCLD, handler_signal);
	signal(SIGQUIT, SIG_IGN);
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
		expander(token, my_envp);
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
		if (exec_ast(node, my_envp, &cleanup) == -1)
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
	free_envp(my_envp);
	return (0);
}
