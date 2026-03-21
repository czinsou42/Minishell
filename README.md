*This project has been created as part of the 42 curriculum by czinsou, lebertau*
# MINISHELL

## Description
Minishell is a project all about **creating your very own shell**. Taking bash's behavior as a baseline, it must reproduce several of its commands (in this project, refered to as "built-ins"), handle redirections and pipes, and overall be usable as any other shell would be.  

This project is the common's core biggest so far, and as such, it is important for the code to be readable, modulable, debuggable, and as clean as it can be.

## Instructions
**Compilation**: 
```bash
make
```
**Execution** : 
```bash
./minishell
```

**Checking memory leaks** (readline.supp is not bundled with the project):
```bash
valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all --track-origins=yes --trace-children=yes --trace-children-skip='/bin/*,/usr/bin/*,/usr/local/bin/*' --suppressions=readline.supp -s ./minishell
```

## Resources
[Bash reference manual](https://www.gnu.org/software/bash/manual/bash.html)  
[Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)  
[LucasKuhn minishell-tester](https://github.com/LucasKuhn/minishell_tester)

AI tools were mainly used for restructuring of human-written code, as to follow **The Norm**.  
AI tools were used to generate test cases.  
AI tools were used for guidance on how to debug code.

## Technical choices

**The code is modular**

Each folder maps to one stage of the minishell pipeline. This modular layout improves readability and makes features easier to implement, test, and maintain independently.

To add a module, create a new folder and source list in the `Makefile`, then include it in the global `SRCS` rule once the feature is ready.

*Example:*
```makefile
TEST_FEATURE = test_feature

FEATURE_SRCS = srcs/feature/feature.c \
			srcs/feature/feature_handlers.c \
			srcs/feature/feature_utils.c \
			srcs/feature/feature_token.c

SRCS = srcs/main.c $(LEXER_SRCS) $(AST_SRCS) $(PARSER_SRCS) $(EXPAND_SRCS) $(EXEC_SRCS) $(HEREDOC_SRCS) $(FEATURE_SRCS)

TEST_FEATURE_SRCS = srcs/feature/feature_test.c $(FEATURE_SRCS)

TEST_FEATURE_OBJS = $(TEST_FEATURE_SRCS:.c=.o)
```

**Folder convention**

Each module usually contains a main `.c` file named after the folder, which holds the core logic. The remaining files are utility/helpers for that module.


## Minishell flow

### 1. Lexer

This module reads the line from `readline()` and converts it into a linked list of tokens.

Example:

**echo hello | cat -e** → **[WORD "echo"] → [WORD "hello"] → [PIPE "|"] → [WORD "cat"] → [WORD "-e"] → NULL**

Tokenization defines the basic grammar elements the shell relies on before parsing.

Quote type is tracked here because expansion behavior (step 3) depends on it.

Enums and data structures used for tokenization are defined in the header files.

### 2. Heredoc

This module handles `TOKEN_REDIR_HEREDOC`.
It keeps prompting until the user enters the delimiter (`<< delimiter`).
Input is buffered as it is read, then processing moves to the next heredoc when the delimiter is reached.

### 3. Expand

This module decides when environment variables should be expanded and when text should stay literal.
It uses shell rules (including quote context) and also handles expansion behavior inside heredoc content when required.

### 4. Parser

The parser transforms the token list into executable command structures.

**Example:**

Tokens : [WORD "cat"] [REDIR_IN "<"] [WORD "file"]
		↓ (PARSER)
Structure :
Command : "cat"
Arguments : (none)
Redirections : 
  - Input : "file"

A command unit contains the command name, its arguments, and optional redirections.

**Example**

Input : "grep test < file > output"
```
Command {
	cmd_name: "grep"
	args: ["test"]
	redirections: [
		{type: REDIR_IN, file: "file"},
		{type: REDIR_OUT, file: "output"}
	]
}
```

This is also where logical operators and AST construction are handled (`&&`, `||`, `|`).

#### AST (Abstract Syntax Tree)

An AST is a hierarchical representation of command logic. Instead of keeping a flat token list, commands and operators are organized by execution priority.

**Why build an AST?**

Consider the command: `ls | cat && echo done`

It makes execution order explicit:
- `|` is evaluated first (higher priority)
- `&&` is evaluated after (lower priority), based on previous exit status

**Tree representation:**
```
        AND
       /   \
    PIPE   COMMAND
    /  \     |
  ls  cat  echo
```

**How the AST is built**

The parser searches operators in this order:
1. **Low priority** operators (`||`, `&&`)
2. **High priority** operator (`|`)
3. **Simple commands**

When an operator is found, a node is created and left/right subtrees are built recursively. This keeps low-priority operators near the root (executed later).

**Build example:**

Input : "ls | cat && echo done"

1. Search for `&&` → create an `AND` node
2. Parse left side `ls | cat` recursively → create a `PIPE` node
3. Parse right side `echo done` → create a `COMMAND` node

**Redirections**

Redirections are attached to each command while parsing (`<`, `>`, `>>`, `<<`).
During execution, this redirection list is applied before launching the command.

### 5. Exec

The execution module is where the AST is evaluated.
It performs a depth-first traversal: command leaves first, operators afterward.

**General behavior**

The tree is evaluated recursively by node type:
- **NODE_COMMAND**: `fork()`, apply redirections, then `execve()`
- **NODE_AND**: execute right child only if left exits with status `0`
- **NODE_OR**: execute right child only if left exits with non-zero status
- **NODE_PIPE**: create a pipe and connect stdout/stdin across both commands

**Heredoc execution**

Heredoc content is collected in phase 2.
During execution, `apply_heredoc()` writes that content into a pipe and redirects stdin to it (file-like behavior, in memory).

**Memory management**

After `fork()`, child processes inherit allocated structures (line, tokens, AST).
To avoid Valgrind leaks, a shared `t_cleanup` context is passed around and freed in children before each `exit()`.

## Implemented features

- **Lexer** : Tokenization with quote and operator handling
- **Heredocs** : Interactive input, expansion, and pipe-based execution
- **Expander** : Environment variable substitution
- **Parser** : AST construction for simple commands, AND, OR
- **Execution** : `fork`/`execve` with proper exit status handling
- **Redirections** : `<`, `>`, `>>` supported
- **Memory management** : 0 Valgrind leaks (`definitely lost` and `still reachable`)
- **Pipes** : Structure defined and execution implemented
- **Builtins** : echo, cd, pwd, export, unset, env, exit
- **Signals** : Ctrl+C, Ctrl+D, Ctrl+\
- **`$?` variable** : Exit status of the last command


## Not implemented

- **Wildcards** : `*` expansion
