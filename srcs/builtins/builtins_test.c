/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 16:07:35 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/08 16:35:36 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	t_command	cmd;
	char		*args[] = {"cd", "/tmp", NULL};
	extern char	**envp;

	cmd.argv = args;
	cmd.redirections = NULL;
	printf("Before cd: PWD=%s\n", ft_getenv("PWD", envp));
	builtin_cd(&cmd, envp);
	printf("After cd : PWD=%s\n", ft_getenv("PWD", envp));
	return (0);
}
