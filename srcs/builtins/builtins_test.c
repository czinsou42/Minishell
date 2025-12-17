/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:36:50 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/17 13:12:43 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_parent_builtin(const char *cmd)
{
	(void)cmd;
	return (0);
}

int execute_builtin_parent(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	(void)cmd;
	(void)envp;
	(void)cleanup;
	return (0);
}

int execute_builtin_simple(t_command *cmd, char ***envp)
{
	(void)cmd;
	(void)envp;
	return (0);
}


void	apply_redirections(t_redir *redir)
{
	(void)redir;
}

char	*get_path(char *cmd, char **envp)
{
	(void)cmd;
	(void)envp;
	return (NULL);
}

void	print_command_error(char *cmd, int error_type)
{
	(void)cmd;
	(void)error_type;
}

int	get_exit_code(int status)
{
	return (status);
}

void	cleanup_and_exit(t_cleanup *cleanup, int status)
{
	(void)cleanup;
	exit(status);
}

/* ===== UTILS ===== */

static t_command	make_cmd(char **argv)
{
	t_command	cmd;

	cmd.argv = argv;
	cmd.redirections = NULL;
	return (cmd);
}

/* ===== MAIN TEST ===== */

int	main(void)
{
	char **envp;
    char **export_vars;
    
    envp = malloc(sizeof(char *) * 3);
    envp[0] = ft_strdup("PATH=/bin:/usr/bin");
    envp[1] = ft_strdup("HELLO=world");
    envp[2] = NULL;

    export_vars = malloc(sizeof(char *) * 2);
    export_vars[0] = ft_strdup("HELLO=world");
    export_vars[1] = NULL;
	t_cleanup	cleanup = {0};

	/* ===== echo ===== */
	printf("== Testing echo ==\n");
	char *echo_args[] = {"echo", "-n", "hello", "world", NULL};
	t_command echo_cmd = make_cmd(echo_args);
	builtin_echo(&echo_cmd);
	printf("\n\n");

	/* ===== pwd ===== */
	printf("== Testing pwd ==\n");
	char *pwd_args[] = {"pwd", NULL};
	t_command pwd_cmd = make_cmd(pwd_args);
	builtin_pwd(&pwd_cmd);
	printf("\n");

	/* ===== env ===== */
	printf("== Testing env ==\n");
	char *env_args[] = {"env", NULL};
	t_command env_cmd = make_cmd(env_args);
	builtin_env(&env_cmd, envp);
	printf("\n");

	/* ===== export ===== */
	printf("== Testing export ==\n");
	char *export_args[] = {"export", "TESTVAR=42", NULL};
	t_command export_cmd = make_cmd(export_args);
	builtin_export(&export_cmd, &envp, &export_vars);

	printf("== Export vars after export ==\n");
	print_export_vars(export_vars);
	printf("\n");

	/* ===== unset ===== */
	printf("== Testing unset ==\n");
	char *unset_args[] = {"unset", "TESTVAR", NULL};
	t_command unset_cmd = make_cmd(unset_args);
	builtin_unset(&unset_cmd, &envp, &export_vars);

	printf("== Export vars after unset ==\n");
	print_export_vars(export_vars);
	printf("\n");

	/* ===== exit ===== */
	printf("== Testing exit ==\n");
	char *exit_args[] = {"exit", "42", NULL};
	t_command exit_cmd = make_cmd(exit_args);
	builtin_exit(&exit_cmd, &cleanup);

	return (0);
}