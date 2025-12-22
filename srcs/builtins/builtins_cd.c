/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:45:57 by czinsou           #+#    #+#             */
/*   Updated: 2025/12/22 16:23:31 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env_forcd(char ***envp, char *name, char *value)
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
	return (env_add(envp, name, value));
}

static int	set_pwd_oldpwd(char ***envp, char *oldcwd)
{
	char	newcwd[PATH_MAX];

	if (!getcwd(newcwd, sizeof(newcwd)))
		return (perror("getcwd"), 1);
	if (update_env_forcd(envp, "OLDPWD", oldcwd))
		return (1);
	if (update_env_forcd(envp, "PWD", newcwd))
		return (1);
	return (0);
}

static char	*get_dash_path(char **envp)
{
	char	*oldpwd;
	char	*path;

	oldpwd = ft_getenv("OLDPWD", envp);
	if (!oldpwd)
	{
		printf("minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	printf("%s\n", oldpwd);
	path = ft_strdup(oldpwd);
	return (path);
}

int	builtin_cd(t_command *cmd, char ***envp)
{
	char	oldcwd[PATH_MAX];
	char	*path;
	char	*home;
	int		should_free;

	if (!getcwd(oldcwd, sizeof(oldcwd)))
		return (perror("getcwd"), 1);
	should_free = 0;
	if (!cmd->argv[1])
	{
		home = ft_getenv("HOME", *envp);
		if (!home)
			return (printf("minishell: cd: HOME not set\n"), 1);
		path = ft_strdup(home);
		if (!path)
			return (1);
		should_free = 1;
	}
	else if (!ft_strcmp(cmd->argv[1], "-"))
	{
		path = get_dash_path(*envp);
		if (!path)
			return (1);
		should_free = 1;
	}
	else
		path = cmd->argv[1];
	if (chdir(path) == -1)
		return (printf("minishell: cd: %s: %s\n", path, strerror(errno)), 1);
	if (should_free)
		free(path);
	return (set_pwd_oldpwd(envp, oldcwd));
}
