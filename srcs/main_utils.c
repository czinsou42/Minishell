/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:00:21 by czinsou           #+#    #+#             */
/*   Updated: 2026/02/22 16:29:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void free_command(t_command *cmd)
{
    int i = 0;
    if (!cmd)
        return;
    if (cmd->argv)
    {
        while (cmd->argv[i])
            free(cmd->argv[i++]);
        free(cmd->argv);
    }
    free_redirections(cmd->redirections);
    free(cmd);
}

void    free_pipeline(t_pipeline *p)
{
    t_pipeline *tmp;

    while (p)
    {
        tmp = p->next;	
        free(p);
        p = tmp;
    }
}