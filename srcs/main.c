/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:49:44 by amwahab           #+#    #+#             */
/*   Updated: 2026/02/16 15:47:12 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

void	handler_signal(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

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
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (i > 0)
				free(new_envp[--i]);
			free(new_envp);
			return (NULL);
		}
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
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
	cleanup.last_status = 0;
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
		g_exit_status = exec_ast(node, &my_envp, &cleanup);
		if (g_exit_status == -1)
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
