/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:39:29 by amwahab           #+#    #+#             */
/*   Updated: 2025/10/30 12:28:24 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	(void)argc;
	char *test_cases[] = {
		"Kratos messi $USER",
		"$HOME/documents",
		"Pas de variables",
		"$USER & $HOME",
		"kratos messi $NONEXISTENT",
		"echo \"\"$USER$HOME\"\"",
		"echo \"$$\"",
		"echo \"$\"",
		NULL};

	printf("========== EXPANSION TESTS ==========\n\n");

	for (int i = 0; test_cases[i]; i++)
	{
		printf("Input:  \"%s\"\n", test_cases[i]);

		char *result = expand(test_cases[i], envp);

		if (result)
		{
			printf("Output: \"%s\"\n", result);
			free(result);
		}
		else
		{
			printf("Output: ERROR (malloc failed)\n");
		}
		printf("\n");
	}

	return (0);
}
