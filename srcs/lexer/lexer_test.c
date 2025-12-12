/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 18:27:46 by amwahab           #+#    #+#             */
/*   Updated: 2025/10/15 10:26:50 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_tokens(t_token *tokens);

int main(void)
{
	char *line;
	t_token *tokens;

	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			printf("exit\n");
			clear_history();
			break;
		}
		tokens = lexer(line);
		print_tokens(tokens);
		free(line);
		free_tokens(tokens);
	}
	return (0);
}

void print_tokens(t_token *tokens)
{
	int i = 0;
	t_token *current = tokens;

	if (!tokens)
	{
		printf("no_tokens\n");
		return;
	}

	while (current)
	{
		printf("Token %d: ", i);
		if (current->type == TOKEN_WORD)
			printf("[TOKEN_WORD]");
		else if (current->type == TOKEN_PIPE)
			printf("[TOKEN_PIPE]");
		else if (current->type == TOKEN_REDIR_IN)
			printf("[TOKEN_REDIR_IN]");
		else if (current->type == TOKEN_REDIR_OUT)
			printf("[TOKEN_REDIR_OUT]");
		else if (current->type == TOKEN_REDIR_APPEND)
			printf("[TOKEN_REDIR_APPEND]");
		else if (current->type == TOKEN_REDIR_HEREDOC)
			printf("[TOKEN_REDIR_HEREDOC]");
		else if (current->type == TOKEN_OR)
			printf("[TOKEN_OR]");
		else if (current->type == TOKEN_AND)
			printf("[TOKEN_AND]");
		else if (current->type == TOKEN_LPAREN)
			printf("[TOKEN_LPAREN]");
		else if (current->type == TOKEN_RPAREN)
			printf("[TOKEN_RPAREN]");

		printf(" \"%s\" ", current->str);

		if (current->quote == NO_QUOTE)
			printf("(NO_QUOTE)");
		else if (current->quote == SINGLE_QUOTE)
			printf("(SINGLE_QUOTE)");
		else if (current->quote == DOUBLE_QUOTE)
			printf("(DOUBLE_QUOTE)");

		printf("\n");

		current = current->next;
		i++;
	}
}
