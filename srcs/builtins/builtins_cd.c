/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:45:57 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/08 16:26:10 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_env(char ***envp, char *name, char *value)
{
	int		i;
	int		len;
	char	*new_var;

	len = ft_strlen(name);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], name, len) && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			new_var = ft_strjoin3(name, "=", value);
			if (!new_var)
				return (1);
			(*envp)[i] = new_var;
			return (0);
		}
		i++;
	}
	return (0);
}

static int	set_pwd_oldpwd(char ***envp, char *oldcwd)
{
	char	newcwd[PATH_MAX];

	if (!getcwd(newcwd, sizeof(newcwd)))
		return (perror("getcwd"), 1);
	if (update_env(envp, "OLDPWD", oldcwd))
		return (1);
	if (update_env(envp, "PWD", newcwd))
		return (1);
	return (0);
}

int	builtin_cd(t_command *cmd, char **envp)
{
	char	oldcwd[PATH_MAX];
	char	*path;

	if (!getcwd(oldcwd, sizeof(oldcwd)))
		return (perror("getcwd"), 1);
	if (!cmd->argv[1])
	{
		path = ft_getenv("HOME", envp);
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = cmd->argv[1];
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (set_pwd_oldpwd(&envp, oldcwd));
}
