# MINISHELL

## Architecture du projet

**Le code est modulable**

Chaque dossier correspond à une séquances du minishell, on peut donc parler de programmation modulaire. J'ai fait ce choix pour avoir une meilleure lisibilité de mon code et surtout une meilleure scalabilité, une feature peut être implémenté et testé sans problème. Ensuite, pour isoler un bug et le traiter j'ai crée différente règle à mon makefile pour tester un module __make test_ast__; __make test_parser__ ... Ainsi il est plus facile de traiter et identifier d'où peur provenir le bug.

Pour rajouter un module, il suffit de créer un nouveau dossier, une fois le code terminé, on rajoute les sources du module à une règle du makefile spécifique au module et sans oublier de rajouter le tout à la règle principale une fois la feature opérationnelle.

*Exemple:*
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

**Contenu des dossiers**

Chaque dossier comporter un fichier .c du même nom que le dossier, c'est ce fichier qui contient la fonction princpale et donc la logique principal du module, tout le reste sont des fichiers utils.

Il est nécessaire de suivre cettre strucutre qui permet une lisibilité optimale du projet en cas d'ajout.

## Flow du minishell

### 1. Lexer

Dans ce module, le but est de transformer la ligne du récupérée grâce à la fonction readline et dans le transofmer en une liste chaînée de "tokens".

Un exemple vaut mieux que 1000 mots:

**echo hello | cat -e** → **[WORD "echo"] → [WORD "hello"] → [PIPE "|"] → [WORD "cat"] → [WORD "-e"] → NULL**

Cette fonction a pour but de déterminer à quoi correspond une chaine de caractère, cela est nécessaire car nous devons appliquer des règles de grammaire à notre minishell, il faut donc commencer par défénir les fondations de notre langage. On peut faire une analogie à une langue vivante, pour construire une phrase qui a du sens on a besoin d'un sujet, verbe et complément, pour le shell c'est la même chose !

Il y a un point important à souligner ici, il faut prendre en compte le type de quotes car le comportement de l'expand **(3)** n'est pas le même selon le type de quotes.

Dans le .h il est possible de retrouver les enum ainsi que les structures utilisé pour tokenizer correctement, tout est correctement commenté il est facile de s'y retrouver.

### 2. Heredoc

Dans ce module on s'occupe du cas où l'on tombe sur un token de type `TOKEN_REDIR_HEREDOC`. Le but ici et d'afficher un prompt tant que l'utilisateur n'a pas taper l'EOF qui correspond à la string juste après le `<<`. Il est crucial de le faire à ce moment là car il faut prendre en compte les variables à expand, on reviendra sur ce point juste après. On stock donc au fur et à mesure que l'utilisateur écrit et une fois que l'utilisateur écris exactement la même chose que le délimiteur, on s'arrête et un passe au heredoc suivant s'il y en a un.

### 3. Expand

Commençons par défninir qu'est ce qu'un expand, dans notre environnement système il y a plusieurs variables qu'on peut retrouver grâve à la commande env ou la varaible `char **envp`. Il y a des cas où l'on doit lire littéralement la variable et d'autre où l'on doit la remplacer par son contenu, c'est le rôle du module. Prendre la décision ou non d'expand lui revient, tout en prennons en compte le contenu du **Heredoc** s'il y a des variables à expand.

### 4. Parser

Ce module est crucial pour le bon fonctionnement du minishell, commençons tout d'abord par définir qu'est ce qu'un parsing. C'est un outil qui analyse la structure d'une liste et la transforme en une structure de commande, reprennons l'analogie à une langue vivante si on a **"Le chat mange un coeur de mangue"**, ici le parser comprend Sujet: "Le chat" Verbe: "mange" Complément: "un coeur de mangue". C'est la même chose pour minishell !

**Exemple:**

Tokens : [WORD "cat"] [REDIR_IN "<"] [WORD "file"]
		↓ (PARSER)
Structure :
Commande : "cat"
Arguments : (aucun)
Redirections : 
  - Input : "file"

Une commande est une unité d'exécution qui contient le nom de la commandes, ses arguments et ses redirection s'il y en a.

**Exemple**

Input : "grep test < file > output"
```
Commande {
	cmd_name: "grep"
	args: ["test"]
	redirections: [
		{type: REDIR_IN, file: "file"},
		{type: REDIR_OUT, file: "output"}
	]
}
```

C'est dans cette partie qu'entre la notion d'opérateurs logique et de construction d'ast. En effet, s'il on tombe sur un token de type && ou || ou | on doit construire un ast.

#### AST (Abstract Syntax Tree)

Un AST ou arbre syntaxique abstrait est une représentation hiérarchique de la structure logique d'une commande. Plutôt que de garder une liste linéaire de tokens, on organise les commandes et opérateurs dans une structure d'arbre qui respecte les priorités d'exécution.

**Pourquoi construire un AST ?**

Imaginons la commande : `ls | cat && echo done`

Sans AST, on aurait du mal à savoir dans quel ordre exécuter les choses. L'AST nous permet de représenter clairement que :
- Le pipe (|) doit s'éxecuter en premier (priorité haute)
- Le AND (&&) s'éxecute après, selon le résultat du pipe (priorité basse)

**Représentation de l'arbre :**
```
        AND
       /   \
    PIPE   COMMAND
    /  \     |
  ls  cat  echo
```

**Comment on construit l'AST ?**

Le parser recherche les opérateurs dans un ordre précis :
1. D'abord les opérateurs de **basse priorité** (||, &&)
2. Ensuite les opérateurs de **haute priorité** (|)
3. Finalement les **commandes simples**

Quand on trouve un opérateur, on crée un noeud pour cet opérateur et on construit récursivement les sous-arbres gauche et droit. Cette approche récursive garantit que les opérateurs de basse priorité se retrouvent en haut de l'arbre (racine) et seront donc éxecutés en dernier.

**Exemple de construction :**

Input : "ls | cat && echo done"

1. On cherche && → Trouvé ! On crée un noeud AND
2. Partie gauche "ls | cat" → On cherche récursivement
   - On trouve | → On crée un noeud PIPE avec ls à gauche et cat à droite
3. Partie droite "echo done" → C'est une commande simple, on crée un noeud COMMAND

**Les redirections**

Les redirections sont gérées au niveau des commandes individuelles. Quand on parse une commande, on identifie tous les opérateurs de redirection (<, >, >>, <<) et on construit une liste chaînée de redirections attachée à la commande. Cette liste sera parcourue lors de l'exécution pour appliquer chaque redirection avant de lancer la commande.

### 5. Exec

Le module d'exécution est le coeur du minishell, c'est ici qu'on donne vie à l'AST qu'on a construit. L'éxecution se fait par un parcours en profondeur de l'arbre, ce qui signifie qu'on éxecute d'abord les feuilles (commandes) avant de remonter vers la racine (opérateurs).

**Principe général**

On parcourt l'arbre récursivement selon le type de noeud :
- **NODE_COMMAND** : On crée un processus enfant avec `fork()`, on applique les redirections, puis on exécute la commande avec `execve()`
- **NODE_AND** : On éxecute le fils gauche, et seulement si il réussit (code retour = 0), on éxecute le fils droit
- **NODE_OR** : On éxecute le fils gauche, et seulement si il échoue (code retour ≠ 0), on éxecute le fils droit
- **NODE_PIPE** : On crée un pipe, lance les deux commandes en parallèle en connectant leur sortie/entrée

**Le cas des heredocs**

Pour les heredocs, on a déjà lu et stocké le contenu pendant la phase 2. Lors de l'éxecution, dans `apply_heredoc()`, on crée un pipe, on écrit le contenu dans le pipe, et on redirige l'entrée standard vers ce pipe. C'est exactement comme si le contenu venait d'un fichier, sauf qu'il est en mémoire.

**Gestion mémoire critique**

Un point crucial de l'éxecution concerne la gestion mémoire. Quand on fait `fork()`, le processus enfant hérite de toutes les structures allouées (tokens, AST, line). Si ce processus termine avec `exit()` sans libérer ces structures, elles apparaissent comme des leaks dans Valgrind.

La solution : on propage une structure **t_cleanup** qui contient les pointeurs vers toutes les allocations principales. Dans le processus enfant, avant chaque `exit()`, on libère explicitement :
- La ligne readline
- La liste de tokens
- L'arbre syntaxique complet

Cette approche garantit 0 bytes de leaks, même dans les processus enfants qui terminent après un échec d'execve.

## Fonctionnalités implémentées

### ✅ Complètement terminé
- **Lexer** : Tokenisation avec gestion des quotes et opérateurs
- **Heredocs** : Lecture interactive, expansion, exécution par pipe
- **Expander** : Remplacement des variables d'environnement
- **Parser** : Construction d'AST pour commandes simples, AND, OR
- **Exécution** : fork/execve avec gestion des codes de retour
- **Redirections** : <, >, >> fonctionnels
- **Gestion mémoire** : 0 leaks Valgrind (definitely lost ET still reachable)

### ⚠️ En cours
- **Pipes** : Structure définie, éxecution à implémenter

### ❌ À faire
- **Builtins** : echo, cd, pwd, export, unset, env, exit
- **Signaux** : Ctrl+C, Ctrl+D, Ctrl+\
- **Variable $?** : Code de retour de la dernière commande
- **Wildcards** : Expansion de * si requis par le sujet

## Compilation et utilisation

Pour compiler le projet :
```bash
make
```

Pour lancer le minishell :
```bash
./minishell
```

Pour tester avec Valgrind (vérification des leaks) :
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell
```

## Tests modulaires

Le Makefile contient des règles pour tester chaque module isolément :
```bash
make test_lexer    # Teste uniquement le lexer
make test_parser   # Teste uniquement le parser
make test_expand   # Teste uniquement l'expander
```

Ces tests permettent d'identifier rapidement d'où provient un bug sans avoir à lancer le minishell complet.

## Instructions pour continuer le développement

### Prochaine étape : Implémentation des pipes

Les pipes nécessitent de créer un `pipe()` système, lancer deux processus en parallèle, et connecter la sortie du premier à l'entrée du second. La fonction `exec_pipeline` existe déjà dans le header mais n'est pas implémentée.

**Ce qu'il faut faire :**
1. Décommenter la ligne dans `exec_ast` qui appelle `exec_pipeline`
2. Implémenter `exec_pipeline` en suivant la logique des heredocs (qui utilisent déjà des pipes)
3. Gérer la fermeture correcte des descripteurs de fichiers
4. Propager la structure cleanup pour éviter les leaks

### Ensuite : Les builtins

Les builtins sont des commandes executées directement par le shell sans créer de processus enfant. Il faudra détecter si la commande est un builtin avant de faire `fork()`, et si oui, executer la fonction correspondante directement.

**Liste des builtins à implémenter :**
- `echo` avec option -n
- `cd` avec chemin relatif/absolu
- `pwd` sans options
- `export` pour ajouter/modifier des variables d'environnement
- `unset` pour supprimer des variables d'environnement
- `env` pour afficher l'environnement
- `exit` avec code de retour optionnel

### Enfin : Signaux et finitions

La gestion des signaux (SIGINT pour Ctrl+C notamment) nécessite d'utiliser `signal()` ou `sigaction()`. Attention à bien gérer les signaux dans le processus parent ET dans les processus enfants.

## Auteurs

Projet réalisé par **amwahab** dans le cadre du cursus 42.