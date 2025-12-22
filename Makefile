# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/12 11:54:28 by amwahab           #+#    #+#              #
#    Updated: 2025/12/22 17:58:38 by czinsou          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
TEST_LEXER = test_lexer
TEST_AST = test_ast
TEST_PARSER = test_parser
TEST_EXPAND = test_expand
TEST_BUILTINS = test_builtins

CC = cc
CFLAGS = -Wall -Werror -Wextra

LEXER_SRCS = srcs/lexer/lexer.c \
			srcs/lexer/lexer_handlers.c \
			srcs/lexer/lexer_utils.c \
			srcs/lexer/lexer_token.c

AST_SRCS = srcs/ast/ast_node.c \
			srcs/ast/ast_utils.c 

PARSER_SRCS = srcs/parser/parser.c \
			srcs/parser/parser_utils.c \
			srcs/parser/parser_command.c \
			srcs/parser/parser_command_utils.c

EXPAND_SRCS = srcs/expander/expand.c \
			srcs/expander/expand_utils.c

EXEC_SRCS = srcs/exec/exec_and.c \
			srcs/exec/exec_command.c \
			srcs/exec/exec_or.c \
			srcs/exec/exec_path.c \
			srcs/exec/exec_redir_utils.c \
			srcs/exec/exec_redir.c \
			srcs/exec/exec_utils.c \
			srcs/exec/exec.c \
			srcs/exec/exec_pipeline_utils.c \
			srcs/exec/exec_pipeline.c \

BUILTINS_SRCS = srcs/builtins/builtins_cd.c \
				srcs/builtins/builtins_utils.c \
				srcs/builtins/builtins_export.c \
				srcs/builtins/builtins_unset.c \
				srcs/builtins/builtins_exit.c \
				srcs/builtins/builtins_simple.c \
				srcs/builtins/builtins.c \

HEREDOC_SRCS = srcs/heredoc/heredoc.c

SRCS = srcs/main.c $(LEXER_SRCS) $(AST_SRCS) $(PARSER_SRCS) $(EXPAND_SRCS) $(EXEC_SRCS) $(HEREDOC_SRCS) $(BUILTINS_SRCS)
TEST_LEXER_SRCS = srcs/lexer/lexer_test.c $(LEXER_SRCS)
TEST_AST_SRCS = srcs/ast/ast_test.c $(AST_SRCS)
TEST_PARSER_SRCS = srcs/parser/parser_test.c $(LEXER_SRCS) $(AST_SRCS) $(PARSER_SRCS)
TEST_EXPAND_SRCS = srcs/expander/expand_test.c $(EXPAND_SRCS) $(LEXER_SRCS)
TEST_BUILTINS_SRCS = srcs/builtins/builtins_test.c $(BUILTINS_SRCS)


OBJS = $(SRCS:.c=.o)
TEST_LEXER_OBJS = $(TEST_LEXER_SRCS:.c=.o)
TEST_AST_OBJS = $(TEST_AST_SRCS:.c=.o)
TEST_PARSER_OBJS = $(TEST_PARSER_SRCS:.c=.o)
TEST_EXPAND_OBJS = $(TEST_EXPAND_SRCS:.c=.o)
TEST_BUILTINS_OBJS = $(TEST_BUILTINS_SRCS:.c=.o)


LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

test_ast: $(TEST_AST)

test_builtins : $(TEST_BUILTINS)


$(TEST_PARSER): $(TEST_PARSER_OBJS) $(LIBFT)
	$(CC) $(TEST_PARSER_OBJS) $(LIBFT) -lreadline -o $(TEST_PARSER)

$(TEST_LEXER): $(TEST_LEXER_OBJS) $(LIBFT)
	$(CC) $(TEST_LEXER_OBJS) $(LIBFT) -lreadline -o $(TEST_LEXER)

$(TEST_AST): $(TEST_AST_OBJS) $(LIBFT)
	$(CC) $(TEST_AST_OBJS) $(LIBFT) -o $(TEST_AST)

$(TEST_EXPAND): $(TEST_EXPAND_OBJS) $(LIBFT)
	$(CC) $(TEST_EXPAND_OBJS) $(LIBFT) -lreadline -o $(TEST_EXPAND)
	
$(TEST_BUILTINS): $(TEST_BUILTINS_OBJS) $(LIBFT)
	$(CC) $(TEST_BUILTINS_OBJS) $(LIBFT) -lreadline -o $(TEST_BUILTINS)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I includes -I $(LIBFT_DIR) -c $< -o $@
	
$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS) $(TEST_LEXER_OBJS) $(TEST_AST_OBJS) $(TEST_PARSER_OBJS) $(TEST_EXPAND_OBJS) $(TEST_BUILTINS_OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME) $(TEST_LEXER) $(TEST_AST) $(TEST_PARSER) $(TEST_EXPAND) $(TEST_BUILTINS)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all test_lexer test_ast test_expand test_builtins clean fclean re