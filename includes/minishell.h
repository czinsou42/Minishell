/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:50:03 by amwahab           #+#    #+#             */
/*   Updated: 2025/12/08 16:34:50 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <libft.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <linux/limits.h>

/*=============================== CODE ERREUR =====================================*/

#define REDIR_ERROR ((t_redir *)-1)


/* ================================= QUOTES ====================================*/

typedef enum e_quote_type {
	NO_QUOTE,		// Pas de quotes
	SINGLE_QUOTE,	// '...'
	DOUBLE_QUOTE,	// "..."
}	t_quote_type;

/* ============================ TOKEN ============================== */

typedef enum e_token_type {
	TOKEN_WORD,				//0 - Mots simples
	TOKEN_PIPE,				//1 - |
	TOKEN_REDIR_IN,			//2 - <
	TOKEN_REDIR_OUT,		//3 - >
	TOKEN_REDIR_APPEND,		//4 - >>
	TOKEN_REDIR_HEREDOC,	//5 - <<
	TOKEN_OR,				//6 - ||
	TOKEN_AND,				//7 - &&
	TOKEN_LPAREN,			//8 - (
	TOKEN_RPAREN,			//9 - )
	TOKEN_ERROR,			//10 ERROR
}	t_token_type;


typedef struct s_token {
	t_token_type	type;				// Type du token
	t_quote_type	quote;				// Mode Quote
	char			*str;				// Contenu (malloc)
	char			*heredoc_content;	// Contenu du HEREDOC
	struct s_token	*next;				// Liste chaînée
}	t_token;

/*================================= REDIR ========================================*/

typedef enum e_redir_type {
	REDIR_NONE,
	REDIR_IN,		//2 - <
	REDIR_OUT,		//3 - >
	REDIR_APPEND,	//4 - >>
	REDIR_HEREDOC,	//5 - <<
}	t_redir_type;

typedef struct s_redir {
	t_redir_type	type;				// Type de redirections
	char			*file;				// Nom du fichier cible
	char			*heredoc_content;	// HEREDOC BUFFER
	struct s_redir	*next;				// Liste chaînée
}	t_redir;

/*================================== COMMANDES ====================================*/

typedef struct s_command {
	char			**argv;			// Tableau : ["cmd", "arg1", ..., NULL]
	t_redir			*redirections;	// {REDIR_TYPE, "file", next}
} t_command;

/*==================================== AST NODE ======================================*/

typedef enum e_node_type {
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}	t_node_type;

typedef struct s_node {
	t_node_type		type;
	t_command		*command;	// Pour NODE_COMMAND
	struct s_node	*left;		// Pour opérateur
	struct s_node	*right;		// Pour opérateur
}	t_node;

/*======================================== OPERATEURS ===================================*/

typedef enum e_operator_prio {
	LOW_PRIO,
	MEDIUM_PRIO,
}	t_operator_prio;


typedef struct s_operator_info {
	int				position;	// Position de l'opérateur trouvé dans la liste
	t_token_type	type;		// TOKEN_OR, TOKEN_AND, ...
}	t_operator_info;

/*====================================== PIPELINE ===========================================*/

// Extract from *cmd from NODE_PIPE sera stocke dans cette liste chainee
typedef struct s_pipeline{
	t_command			*command;
	struct s_pipeline	*next;
}	t_pipeline;

/*==================================== CLEAN UP ==============================================*/

typedef struct s_cleanup {
	char	*line;		// Ligne de readline à libérer
	t_token	*tokens;	// Liste de tokens à libérer
	t_node	*ast;		// Arbre syntaxique à libérer
}	t_cleanup;
/*==================================================================================*/
/*============================= FONCTIONS ==========================================*/
/*==================================================================================*/

/*================================ LEXER ===========================================*/

t_token	*lexer(char *line); 
/* ******************************************************************************* */
/* Liste chaînée de tokens :                                                       */
/* [WORD "echo"] → [WORD "hello"] → [PIPE "|"] → [WORD "cat"] → [WORD "-e"] → NULL */
/* ******************************************************************************  */

// TOKEN
t_token	*create_token(t_token_type type, t_quote_type quote, char *str, int len);
void	token_add_back(t_token **lst, t_token *new);
void	free_tokens(t_token *tokens);

// TOKEN_HANDLER
int	handle_operator(char *line, int *i, t_token **tokens);
int	handle_word(char *line, int *i, t_token **tokens);
int	handle_quotes(char *line, int *i, t_token **tokens);

//TOKEN UTILS
void	skip_spaces(char *line, int *i);
t_token_type	get_operator_type(char *str);
t_quote_type	get_quote_type(char *line, int *i, char *quote_char);

//QUOTES
t_quote_type	get_quote_type(char *line, int *i, char *quote_char);

/*===================================== HEREDOC ======================================*/

int		process_heredoc(t_token *tokens);

/*===================================== EXPANDER ========================================*/

// EXPAND
void	expander(t_token *tokens, char **envp);
char	*expand(char *str, char **envp);

// UTILS
int	get_var_len(char *str, int *i, char **envp);
int	expanded_len(char *str, char **envp);

int	copy_var_value(char *str, int *i, char *result, int k, char **envp);
char	*copy_expanded_str(char *str, char *result, char **envp);

char *ft_getenv(char *name, char **envp);

int	is_valid_var_char(char c);

/*======================================= AST =============================================*/

// NODE
t_node	*create_node(t_node_type type, t_command *command);
int		count_nodes(t_node *node);

// AST UTILS
void	print_ast(t_node *node, int depth);
void	free_ast(t_node *node);
void	free_redirections(t_redir *redirections);

/*====================================== PARSING ===========================================*/

t_node	*parse(t_token *tokens, int length);

// PARSE COMMAND
t_command		*parse_command(t_token *tokens, int length);

// PARSE COMMAND UTILS
char			**create_argv(t_token *tokens, int count, int length);
t_redir			*parse_redirections(t_token *tokens, int length);
void			redir_add_back(t_redir **lst, t_redir *new);
void			print_unexpected_token(void);
int				print_parser_redir_error(t_token *token);

// UTILS
t_operator_info	find_operator(t_token *tokens, int length, t_operator_prio prio);
int				has_wrapping_parentheses(t_token *tokens, int length);
int				is_target_operator(t_token_type type, t_operator_prio prio);
t_node			*handle_operator_parser(t_token *tokens, int length, 
					t_operator_info operator_info);
t_node			*create_command_node(t_token *tokens, int length);
void			print_syntax_error(char *token);



// TOKENS
int				ft_tokens_size(t_token *lst);
int				count_tokens_word(t_token *token, int length);
t_redir_type	token_to_redir_type(t_token_type type);
t_token			*advance_token(t_token *token, int position);

/*========================================= EXEC ===============================================*/

// MAIN EXECS
int	exec_ast(t_node *node, char **envp, t_cleanup *cleanup);
int	exec_pipeline(t_node *node, char **envp, t_cleanup *cleanup);
int	exec_command(t_command *cmd, char **envp, t_cleanup *cleanup);
int	exec_or(t_node *node, char **envp, t_cleanup *cleanup);
int	exec_and(t_node *node, char **envp, t_cleanup *cleanup);

// PATH
char	*get_path(char *cmd, char **envp);
char	*find_path_in_env(char **envp);
char	*try_path(char **paths, char *cmd);

// REDIR
void	apply_redirections(t_redir *redir);
void	print_redir_error(char *file);
void	apply_in(t_redir *redir);
void	apply_out(t_redir *redir);
void	apply_append(t_redir *redir);
void	apply_heredoc(t_redir *redir);

// UTILS
void	print_command_error(char *cmd, int error_type);
int		get_exit_code(int status);

/*========================================= BUILTINS ===============================================*/

int	builtin_cd(t_command *cmd, char **envp);
int	exec_command(t_command *cmd, char **envp, t_cleanup *cleanup);
char	*ft_getenv(char *name, char **envp);
char	*ft_strjoin3(char *a, char *b, char *c);
int	is_parent_builtin(const char *cmd);
int	execute_builtin_simple(t_command *cmd);
//int	execute_builtin_parent(t_command *cmd, char ***envp, t_cleanup *cleanup);


#endif