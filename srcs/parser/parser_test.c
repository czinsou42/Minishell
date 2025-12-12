/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@42.student.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:14:44 by amwahab           #+#    #+#             */
/*   Updated: 2025/10/22 12:12:23 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

char	*get_token_type_name(t_token_type type);
void	print_tokens(t_token *tokens);

void	print_tokens(t_token *tokens)
{
	t_token *current = tokens;
	int i = 0;

	printf("\n========== TOKENS ==========\n");
	while (current)
	{
		printf("[%d] Type: %-15s | Content: '%s'\n",
			   i,
			   get_token_type_name(current->type),
			   current->str);
		current = current->next;
		i++;
	}
	printf("============================\n");
}

char	*get_token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return "WORD";
	if (type == TOKEN_PIPE)
		return "PIPE";
	if (type == TOKEN_REDIR_IN)
		return "REDIR_IN";
	if (type == TOKEN_REDIR_OUT)
		return "REDIR_OUT";
	if (type == TOKEN_REDIR_APPEND)
		return "REDIR_APPEND";
	if (type == TOKEN_REDIR_HEREDOC)
		return "REDIR_HEREDOC";
	if (type == TOKEN_AND)
		return "AND";
	if (type == TOKEN_OR)
		return "OR";
	if (type == TOKEN_LPAREN)
		return "LPAREN";
	if (type == TOKEN_RPAREN)
		return "RPAREN";
	return "UNKNOWN";
}

int	main(void)
{
	char *line;
	t_token *tokens;
	t_node *ast;
	int length;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break;
		if (ft_strlen(line) == 0)
		{
			free(line);
			continue;
		}
		add_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}
		tokens = lexer(line);
		if (!tokens)
		{
			printf("Lexer error\n");
			free(line);
			continue;
		}
		print_tokens(tokens);
		length = ft_tokens_size(tokens);
		ast = parse(tokens, length);

		if (!ast)
		{
			printf("Parser error (see message above)\n");
		}
		else
		{
			printf("\n========== AST ==========\n");
			print_ast(ast, 0);
			printf("=========================\n");
		}
		free_tokens(tokens);
		free_ast(ast);
		free(line);

		printf("\n");
	}
	return (0);
}
