# Parser

Le parser prend la liste de tokens produite par le lexer et construit un **AST** (Abstract Syntax Tree — arbre syntaxique abstrait). L'AST représente la structure hiérarchique de la commande : pipes, opérateurs logiques, redirections, sous-shells.

---

## Structures de données

```c
// Types de redirections
typedef enum e_redir_type
{
    REDIR_IN,      // <  lecture depuis un fichier
    REDIR_OUT,     // >  écriture (écrase)
    REDIR_APPEND,  // >> écriture (ajoute)
    REDIR_HEREDOC  // << lecture jusqu'au délimiteur
} t_redir_type;

// Une redirection (liste chaînée)
typedef struct s_redir
{
    t_redir_type  type;    // le type de redirection
    char         *target;  // nom du fichier ou délimiteur heredoc
    struct s_redir *next;  // redirection suivante
} t_redir;

// Types de nœuds de l'AST
typedef enum e_node_type
{
    NODE_CMD,      // feuille : commande simple avec ses arguments
    NODE_PIPE,     // branche : cmd_gauche | cmd_droite
    NODE_AND,      // branche : cmd_gauche && cmd_droite  (bonus)
    NODE_OR,       // branche : cmd_gauche || cmd_droite  (bonus)
    NODE_SUBSHELL  // branche : (commandes…)              (bonus)
} t_node_type;

// Une commande simple
typedef struct s_cmd
{
    char   **argv;          // tableau d'arguments, argv[0] = la commande
    t_redir *redirections;  // liste chaînée de redirections
} t_cmd;

// Un nœud de l'AST
typedef struct s_ast
{
    t_node_type   type;   // CMD, PIPE, AND, OR ou SUBSHELL
    t_cmd        *cmd;    // rempli seulement si type == NODE_CMD
    struct s_ast *left;   // enfant gauche (ou unique enfant pour SUBSHELL)
    struct s_ast *right;  // enfant droit (NULL pour SUBSHELL et CMD)
} t_ast;
```

---

## Grammaire implémentée

Le parser suit une **grammaire descendante récursive** avec cette hiérarchie de priorité (du plus faible au plus fort) :

```
logical    →  pipeline  ( ('&&' | '||')  pipeline )*
pipeline   →  command   ( '|'  command )*
command    →  '(' logical ')'           ← sous-shell
           |  WORD+ redirection*        ← commande simple
```

---

## Point d'entrée : `parser.c`

### `parse_logical`

```c
t_ast *parse_logical(t_gc *gc, t_token **tokens)
{
    t_ast *left = parse_pipeline(gc, tokens); // parse le membre gauche

    // si le token suivant est && ou ||
    if ((*tokens)->type == TOK_AND || (*tokens)->type == TOK_OR)
    {
        t_token_type op = (*tokens)->type;
        *tokens = (*tokens)->next; // consomme le token opérateur

        // crée un nœud AND ou OR
        t_ast *node = make_node(gc, op == TOK_AND ? NODE_AND : NODE_OR);
        node->left  = left;
        node->right = parse_pipeline(gc, tokens); // parse le membre droit
        return (node);
    }
    return (left); // pas d'opérateur logique → retourne directement
}
```

### `parse_pipeline`

```c
t_ast *parse_pipeline(t_gc *gc, t_token **tokens)
{
    t_ast *left = parse_command(gc, tokens); // parse la première commande

    // tant qu'il y a des '|', on construit des nœuds PIPE
    while ((*tokens)->type == TOK_PIPE)
    {
        t_ast *pipe_node = make_node(gc, NODE_PIPE);
        pipe_node->left  = left;
        *tokens = (*tokens)->next;            // consomme le '|'
        pipe_node->right = parse_command(gc, tokens); // commande droite
        left = pipe_node; // le pipe devient le nouveau "gauche" (association gauche)
    }
    return (left);
}
```

**Exemple** : `ls | grep foo | wc -l` produit :
```
       PIPE
      /    \
   PIPE    CMD(wc -l)
  /    \
CMD(ls) CMD(grep foo)
```

---

## Analyse des commandes : `parser_cmd.c`

### `parse_command` — aiguillage

```c
t_ast *parse_command(t_gc *gc, t_token **tokens)
{
    // si on voit '(' → c'est un sous-shell
    if ((*tokens)->type == TOK_LPAREN)
        return parse_subshell(gc, tokens);
    else
        return parse_simple_command(gc, tokens);
}
```

### Sous-shell

```c
static t_ast *parse_subshell(t_gc *gc, t_token **tokens)
{
    t_ast *node = make_node(gc, NODE_SUBSHELL);
    *tokens = (*tokens)->next;          // consomme '('
    node->left = parse_logical(gc, tokens); // parse récursivement le contenu
    *tokens = (*tokens)->next;          // consomme ')'
    return (node);
}
```

### Commande simple

```c
static t_ast *parse_simple_command(t_gc *gc, t_token **tokens)
{
    size_t count = count_cmd_words(*tokens); // compte les mots (hors redirections)

    t_cmd *cmd   = gc_alloc(gc, sizeof(t_cmd));
    cmd->argv    = gc_alloc(gc, sizeof(char *) * (count + 1)); // +1 pour le NULL final

    t_ast *node  = make_node(gc, NODE_CMD);
    cmd = fill_cmd(gc, tokens, cmd); // remplit argv et redirections
    node->cmd    = cmd;
    return (node);
}
```

### `fill_cmd` — remplit les arguments et redirections

```c
static t_cmd *fill_cmd(t_gc *gc, t_token **tokens, t_cmd *cmd)
{
    size_t i = 0;

    // consomme tant qu'il y a des mots ou des redirections
    while ((*tokens)->type == TOK_WORD || is_redir((*tokens)->type))
    {
        if (is_redir((*tokens)->type))
        {
            // parse la redirection et l'insère en tête de liste
            t_redir *redir = parse_redirection(gc, tokens);
            redir->next = cmd->redirections;
            cmd->redirections = redir;
        }
        else
        {
            cmd->argv[i] = (*tokens)->value; // copie la valeur du mot
            *tokens = (*tokens)->next;
            i++;
        }
    }
    cmd->argv[i] = NULL; // termine le tableau comme argv POSIX
    return (cmd);
}
```

---

## Utilitaires : `parser_utils.c`

```c
// Vérifie si un type de token est une redirection
int is_redir(t_token_type type)
{
    return (type == TOK_REDIR_IN || type == TOK_REDIR_OUT
         || type == TOK_REDIR_APPEND || type == TOK_HEREDOC);
}

// Compte les mots d'une commande (saute les paires token_redir + cible)
size_t count_cmd_words(t_token *tokens)
{
    size_t count = 0;
    while (tokens->type == TOK_WORD || is_redir(tokens->type))
    {
        if (tokens->type == TOK_WORD)
            { tokens = tokens->next; count++; }
        else
            { tokens = tokens->next; tokens = tokens->next; } // saute redir + cible
    }
    return (count);
}

// Convertit un type de token en type de redirection
t_redir_type convert_redir_type(t_token_type type); // TOK_REDIR_IN → REDIR_IN, etc.

// Consomme le token opérateur et le token cible, retourne une t_redir
t_redir *parse_redirection(t_gc *gc, t_token **tokens)
{
    t_redir *r  = gc_alloc(gc, sizeof(t_redir));
    r->type     = convert_redir_type((*tokens)->type); // type de redir
    *tokens     = (*tokens)->next;
    r->target   = (*tokens)->value; // nom du fichier / délimiteur heredoc
    *tokens     = (*tokens)->next;
    r->next     = NULL;
    return (r);
}
```

---

## Construction des nœuds : `parser_ast.c`

```c
t_ast *make_node(t_gc *gc, t_node_type type)
{
    t_ast *ast  = gc_alloc(gc, sizeof(t_ast)); // alloué dans l'arena GC
    ast->type   = type;
    ast->cmd    = NULL;
    ast->left   = NULL;
    ast->right  = NULL;
    return (ast);
}
```

---

## Exemple complet

Entrée : `cat < in.txt | grep foo > out.txt && echo done`

```
                AND
               /   \
            PIPE    CMD(echo done)
           /    \
    CMD(cat)    CMD(grep foo)
    redir:<in.txt   redir:>out.txt
```

Représentation mémoire :
```
ast->type = NODE_AND
ast->left->type = NODE_PIPE
  ast->left->left->type  = NODE_CMD
    cmd->argv = ["cat", NULL]
    cmd->redirections = { REDIR_IN, "in.txt" }
  ast->left->right->type = NODE_CMD
    cmd->argv = ["grep", "foo", NULL]
    cmd->redirections = { REDIR_OUT, "out.txt" }
ast->right->type = NODE_CMD
  cmd->argv = ["echo", "done", NULL]
```
