/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:36:50 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/14 12:41:11 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_redir *redir)
{
	(void)redir;
}

char	*get_path(char *cmd, char **envp)
{
	(void)envp;
	return (cmd);
}

void	print_command_error(char *cmd, int error_type)
{
	(void)error_type;
	printf("Command error: %s\n", cmd);
}

int get_exit_code(int status) 
{
	 return status; 
}

void	cleanup_and_exit(t_cleanup *cleanup, int status)
{
	(void)cleanup;
	printf("Exiting with status %d\n", status);
	exit(status);
}

int	is_parent_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0);
}


int	execute_builtin_simple(t_command *cmd, char ***envp)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(cmd));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(cmd, *envp));
	return (1);
}


int	execute_builtin_parent(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd, *envp));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd, envp, NULL));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd, envp, NULL));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd, cleanup));
	return (-1);
}

char **init_env(void)
{
    char **envp = malloc(3 * sizeof(char *));
    envp[0] = ft_strdup("PATH=/bin");
    envp[1] = ft_strdup("HOME=/home/charbel");
    envp[2] = NULL;
    return envp;
}

char **init_export(void)
{
    char **export_vars = malloc(1 * sizeof(char *));
    export_vars[0] = NULL;
    return export_vars;
}

void free_split(char **split)
{
    int i = 0;
    if (!split)
        return;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

// ---------- Main test ----------
int main(void)
{
    t_command cmd_export;
    t_command cmd_unset;

    // Simule les argv pour export et unset
    char *argv_export[] = {"export", "TESTVAR=42", "HELLO", NULL};
    char *argv_unset[] = {"unset", "TESTVAR", NULL};

    cmd_export.argv = argv_export;
    cmd_unset.argv = argv_unset;

    char **envp = init_env();
    char **export_vars = init_export();

    printf("== Testing export ==\n");
    builtin_export(&cmd_export, &envp, &export_vars);

    printf("== Export vars after export ==\n");
    for (int i = 0; export_vars[i]; i++)
        printf("%s\n", export_vars[i]);

    printf("== Testing unset ==\n");
    builtin_unset(&cmd_unset, &envp, &export_vars);

    printf("== Export vars after unset ==\n");
    for (int i = 0; export_vars[i]; i++)
        printf("%s\n", export_vars[i]);

    // Libération de la mémoire
    free_split(envp);
    free_split(export_vars);

    return 0;
}